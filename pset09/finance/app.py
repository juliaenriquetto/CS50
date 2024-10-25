import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    transactions_db = db.execute(
        "SELECT symbol, SUM(shares) AS shares, price FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash_db[0]["cash"]

    return render_template("index.html", database=transactions_db, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Validate symbol input
        if not symbol:
            return apology("Must give symbol")

        # Validate shares input
        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Shares must be a positive integer")

        shares = int(shares)
        stock = lookup(symbol.upper())

        if stock is None:
            return apology("Symbol doesn't exist")

        transaction_value = round(shares * stock["price"], 2)

        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = user_cash_db[0]["cash"]

        # Validate sufficient funds
        if user_cash < transaction_value:
            return apology("Not enough money")

        # Update user's cash
        update_cash = round(user_cash - transaction_value, 2)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", update_cash, user_id)

        date = datetime.datetime.now()

        try:
            db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                       user_id, stock["symbol"], shares, stock["price"], date.strftime("%Y-%m-%d %H:%M:%S"))
        except Exception as e:
            return apology(f"Error saving transaction: {e}")

        # Display success message
        flash("Bought")

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions_db = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html", transactions=transactions_db)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol").upper()

        if not symbol:
            return apology("Must give symbol")

        stock = lookup(symbol)

        if stock is None:
            return apology("Symbol doesn't exist")

        # Formatar o preço com duas casas decimais
        price = f"{stock['price']:.2f}"

        # Renderizar o template quoted.html com os valores
        return render_template("quoted.html", name=stock["name"], price=price, symbol=stock["symbol"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("MUST give username!")

        if not password:
            return apology("MUST give password!")

        if not confirmation:
            return apology("MUST give confirmation!")

        if password != confirmation:
            return apology("Passwords don't match.")

        hash = generate_password_hash(password)

        try:
            new_user = db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hash)
        except:
            return apology("Username already exists")

        session["user_id"] = new_user

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        user_id = session["user_id"]

        # Obter símbolos de ações que o usuário possui
        symbols_user = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id
        )

        return render_template("sell.html", symbols=symbols_user)

    else:
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Verificar se a entrada é válida
        if not symbol:
            return apology("Must provide symbol")
        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("Shares must be a positive integer")

        shares = int(shares)
        stock = lookup(symbol.upper())

        if stock is None:
            return apology("Symbol doesn't exist")

        user_id = session["user_id"]

        # Obter o número de ações que o usuário possui
        user_shares = db.execute(
            "SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, symbol
        )

        # Verificar se o usuário possui ações suficientes para vender
        if len(user_shares) == 0 or user_shares[0]["shares"] < shares:
            return apology("Not enough shares")

        # Calcular o valor da venda
        sale_value = round(shares * stock["price"], 2)

        # Atualizar o saldo de caixa do usuário
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = user_cash_db[0]["cash"]
        update_cash = round(user_cash + sale_value, 2)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", update_cash, user_id)

        # Registrar a transação de venda
        date = datetime.datetime.now()
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
            user_id, stock["symbol"], -shares, stock["price"], date.strftime("%Y-%m-%d %H:%M:%S")
        )

        # Exibir uma mensagem de sucesso
        flash("Sold")

        return redirect("/")

# personal touch - add more cash


@app.route("/add", methods=["GET", "POST"])
@login_required
def add_cash():
    """User can add cash"""
    if request.method == "GET":
        return render_template("add.html")
    else:
        how_much = int(request.form.get("how_much"))

        if not how_much:
            return apology("You must give money")

        user_id = session["user_id"]
        user_cash_db = db.execute("SELECT cash FROM users WHERE id = ?", user_id)  # current cash
        user_cash = user_cash_db[0]["cash"]  # Catch first argument in cash

        # Update user's cash
        update_cash = user_cash + how_much
        db.execute("UPDATE users SET cash = ? WHERE id = ?", update_cash, user_id)

        return redirect("/")
