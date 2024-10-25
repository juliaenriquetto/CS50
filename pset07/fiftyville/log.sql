-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Theft took place on July 28, 2023
-- Theft took place on Humphrey Street

-- suspects:

SELECT description FROM crime_scene_reports WHERE month = 7 AND day = 28 AND street = 'Humphrey Street';
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Three witnesses mentions the bakery
-- littering took place at 16:36. No witnesses.

-- Interviews that mentions the bakery
SELECT * from interviews WHERE transcript LIKE '%bakery%';
-- Ruth: saw the thief in the parking lot of the bakery - 10 minutes after he arrives
-- Eugene: ATM - Leggett Street withdrawing some money
-- Raymond: phone call - they said were planning to take the earliest flight out of fiftyville tomorrow. Said for someone to purchase the flight ticket.

-- Witness 1: Ruth
SELECT * FROM bakery_security_logs WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25;

-- check against license_plate
SELECT p.name, bakery.activity, bakery.license_plate, bakery.year, bakery.month, bakery.day, bakery.hour, bakery.minute
FROM bakery_security_logs bakery
JOIN people p ON p.license_plate = bakery.license_plate
WHERE bakery.year = 2023 AND bakery.month = 7 AND bakery.day = 28 AND bakery.hour = 10 AND bakery.minute BETWEEN 15 AND 25;
-- witness for the thief:
-- Vanessa, Bruce, Barry, Luca, Sofia, Iman, Diana, Kelsey

-- Witness 2: Eugene
SELECT * FROM atm_transactions
WHERE atm_location = 'Leggett Street'
AND year = 2023 AND month = 7 AND day = 28;

-- Witness 3: Raymond
SELECT * FROM phone_calls
WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60;

-- Get the name from the caller and the receiver
SELECT p.name, pc.caller, pc.receiver, pc.year, pc.month, pc.day, pc.duration FROM phone_calls pc
JOIN people p ON pc.caller = p.phone_number
WHERE pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60;
-- *Sofia, *Kelsey, *Bruce, *Kelsey, Taylor, Diana, Carina, Kenny, Benista

-- explore airport
SELECT * from airports;
-- id: 8, abbreviation: CSF, full_name: Fiftyville Regional Airport, city: Fiftyville

-- explore flights
SELECT f.*, origin.full_name AS origin_airport, destination.full_name AS destination_airport
FROM flights f
JOIN airports origin ON f.origin_airport_id = origin.id
JOIN airports destination ON f.destination_airport_id = destination.id
WHERE origin.id = 8 AND f.year = 2023 AND f.month = 7 AND f.day = 28
ORDER BY f.hour, f.minute;
-- LaGuardia Airport, New York

-- combine info from the testimonies
SELECT p.name
FROM bakery_security_logs bsl
JOIN people p ON p.license_plate = bsl.license_plate
JOIN bank_accounts ba ON ba.person_id = p.id
JOIN atm_transactions at ON at.account_number = ba.account_number
JOIN phone_calls pc ON pc.caller = p.phone_number
WHERE bsl.year = 2023 AND bsl.month = 7 AND bsl.day = 28 AND bsl.hour = 10 AND bsl.minute BETWEEN 15 AND 25
AND atm_location = 'Leggett Street' AND at.year = 2023 AND at.month = 7 AND at.day = 28 AND at.transaction_type = 'withdraw'
AND pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60;
-- Bruce and Diana

-- who is on flight?
SELECT p.name
FROM people p
JOIN passengers ps ON p.passport_number = ps.passport_number
WHERE ps.flight_id = 36
AND p.name IN ('Bruce', 'Diana');
-- Bruce

-- Who did Bruce called?
SELECT p2.name AS receiver
FROM phone_calls pc
JOIN people p1 ON pc.caller = p1.phone_number
JOIN people p2 ON pc.receiver = p2.phone_number
WHERE p1.name = 'Bruce' AND pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60;
-- Robin
