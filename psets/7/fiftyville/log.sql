-- Keep a log of any SQL queries you execute as you solve the mystery.
-- First ill query the description of the crime scene report from the date of the crime: July 28, 2021 in the Humphrey Street
SELECT
  description
FROM
  crime_scene_reports
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND street = 'Humphrey Street';

-- The report mentions a crime in the bakery at 10h15, littering hour was 16h36
-- Second, ill query the bakery security logs from around that time, lookin for informationg
SELECT
  HOUR,
  MINUTE,
  activity,
  license_plate
FROM
  bakery_security_logs
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28;

-- At the time of the robbery, 10h16, the license_plate 5P2BI95 exited the bakery, they stayed at least one your inside the stabilishment
-- Theres a table interviews, maybe it has more information about the crime in the day, the crime report say that the witnesses mentioned the bakery, so lets filter the query
SELECT
  name,
  transcript
FROM
  interviews
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND transcript LIKE '%bakery%';

-- Ruth mentioned that around 10h25, the theft got into a car, check the parking lot, check the bakery security cameras at 10h25
-- Eugene mentioned a withdraw by the thief in the Legget Street early in the morning, check the atm
-- Raymond saw the thief say they was gonna pick the earliest fly out to  fiftville in July 29th, the other person bought the tickets, there was also a call when he left the bakery
-- Check again the car plates who left within 10 minutes after the robbery at 10h15
SELECT
  HOUR,
  MINUTE,
  activity,
  license_plate
FROM
  bakery_security_logs
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND activity = 'exit'
  AND HOUR = 10;

-- Two cars left at 10h23: 322W7JE and 0NTHK55
-- Check a call that lasted less than a 1 minute (60 sec) after the robbery
SELECT
  caller,
  receiver
FROM
  phone_calls
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND duration <= 60;

-- +----------------+----------------+
-- |     caller     |    receiver    |
-- +----------------+----------------+
-- | (130) 555-0289 | (996) 555-8899 |
-- | (499) 555-9472 | (892) 555-8872 |
-- | (367) 555-5533 | (375) 555-8161 |
-- | (609) 555-5876 | (389) 555-5198 |
-- | (499) 555-9472 | (717) 555-1342 |
-- | (286) 555-6063 | (676) 555-6554 |
-- | (770) 555-1861 | (725) 555-3243 |
-- | (031) 555-6622 | (910) 555-3251 |
-- | (826) 555-1652 | (066) 555-9701 |
-- | (338) 555-6650 | (704) 555-2131 |
-- +----------------+----------------+
-- Check the ATM in the morning
SELECT
  atm_location
FROM
  atm_transactions;

-- The withdraw was in the leggett street's atm
SELECT
  account_number,
  amount
FROM
  atm_transactions
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND transaction_type = 'withdraw'
  AND atm_location = 'Leggett Street';

-- +----------------+--------+
-- | account_number | amount |
-- +----------------+--------+
-- | 28500762       | 48     |
-- | 28296815       | 20     |
-- | 76054385       | 60     |
-- | 49610011       | 50     |
-- | 16153065       | 80     |
-- | 25506511       | 20     |
-- | 81061156       | 30     |
-- | 26013199       | 35     |
-- +----------------+--------+
-- Should consider the firsts as the earliests
-- Check the airplane tickets for July 29th out of fiftyville
-- First, understand what is the town's airport
SELECT
  *
FROM
  airports;

-- +----+--------------+-----------------------------+------------+
-- | id | abbreviation |          full_name          |    city    |
-- +----+--------------+-----------------------------+------------+
-- | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- +----+--------------+-----------------------------+------------+
-- Get the earliest flight out of fiftyville in July 29th
SELECT
  flights.id,
  HOUR,
  MINUTE
FROM
  flights
  JOIN airports ON flights.origin_airport_id = airports.id
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 29
  AND airports.abbreviation = 'CSF'
ORDER BY
  HOUR ASC,
  MINUTE ASC
LIMIT
  1;

-- +----+------+--------+
-- | id | hour | minute |
-- +----+------+--------+
-- | 36 | 8    | 20     |
-- +----+------+--------+
-- The #36 flight was the earliest one in the day
-- Check the passengers at the flight
SELECT
  *
FROM
  passengers
WHERE
  flight_id = 36;

-- +-----------+-----------------+------+
-- | flight_id | passport_number | seat |
-- +-----------+-----------------+------+
-- | 36        | 7214083635      | 2A   |
-- | 36        | 1695452385      | 3B   |
-- | 36        | 5773159633      | 4A   |
-- | 36        | 1540955065      | 5C   |
-- | 36        | 8294398571      | 6C   |
-- | 36        | 1988161715      | 6D   |
-- | 36        | 9878712108      | 7A   |
-- | 36        | 8496433585      | 7B   |
-- +-----------+-----------------+------+
-- Get all the people who where at the parking lot in the day of the thief, called people in the morning, withdraw money and took a fligth out of the city in the next day
SELECT DISTINCT
  people.name,
  phone_number,
  passport_number,
  license_plate,
  accounts.number AS account_number
FROM
  people
  JOIN (
    SELECT
      license_plate AS plate
    FROM
      bakery_security_logs
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND activity = 'exit'
      AND HOUR = 10
  ) AS licenses ON people.license_plate = licenses.plate
  JOIN (
    SELECT
      caller AS number
    FROM
      phone_calls
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND duration <= 60
  ) AS phones ON people.phone_number = phones.number
  JOIN (
    SELECT
      passport_number AS number
    FROM
      passengers
    WHERE
      flight_id = 36
  ) AS passports ON people.passport_number = passports.number
  JOIN bank_accounts ON people.id = bank_accounts.person_id
  JOIN (
    SELECT
      account_number AS number
    FROM
      atm_transactions
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND transaction_type = 'withdraw'
      AND atm_location = 'Leggett Street'
  ) AS accounts ON bank_accounts.account_number = accounts.number;

-- +--------+----------------+-----------------+---------------+----------------+
-- |  name  |  phone_number  | passport_number | license_plate | account_number |
-- +--------+----------------+-----------------+---------------+----------------+
-- | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       |
-- | Taylor | (286) 555-6063 | 1988161715      | 1106N58       | 76054385       |
-- +--------+----------------+-----------------+---------------+----------------+
-- We got two names, Bruce and Taylor, Lets fact check with the clues we got
-- starting with the car plate
SELECT
  HOUR,
  MINUTE,
  activity,
  license_plate
FROM
  bakery_security_logs
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND activity = 'exit'
  AND HOUR = 10
  AND (
    license_plate = '94KL13X'
    OR license_plate = '1106N58'
  );

-- +------+--------+----------+---------------+
-- | hour | minute | activity | license_plate |
-- +------+--------+----------+---------------+
-- | 10   | 18     | exit     | 94KL13X       |
-- | 10   | 35     | exit     | 1106N58       |
-- +------+--------+----------+---------------+
-- taylor leaves the bakery way to late
-- Check the calls made in the morning
SELECT
  caller,
  receiver
FROM
  phone_calls
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND duration <= 60
  AND (
    caller = '(367) 555-5533'
    OR caller = '(286) 555-6063'
  );

-- +----------------+----------------+
-- |     caller     |    receiver    |
-- +----------------+----------------+
-- | (367) 555-5533 | (375) 555-8161 |
-- | (286) 555-6063 | (676) 555-6554 |
-- +----------------+----------------+
-- This will help to find the accompliance later
-- The withdraws
SELECT
  account_number,
  amount
FROM
  atm_transactions
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND transaction_type = 'withdraw'
  AND atm_location = 'Leggett Street'
  AND (
    account_number = '49610011'
    OR account_number = '76054385'
  );

-- +----------------+--------+
-- | account_number | amount |
-- +----------------+--------+
-- | 76054385       | 60     |
-- | 49610011       | 50     |
-- +----------------+--------+
-- The taylor's withdraw was eralier
-- The passengers at the flight
SELECT
  flights.id,
  origin.city,
  destination.city,
  passengers.passport_number
FROM
  flights
  JOIN airports AS origin ON flights.origin_airport_id = origin.id
  JOIN airports AS destination ON flights.destination_airport_id = destination.id
  JOIN passengers ON flights.id = passengers.flight_id
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 29
  AND origin.abbreviation = 'CSF'
  AND (
    passengers.passport_number = '5773159633'
    OR passengers.passport_number = '1988161715'
  );

-- Both the destinations are to New York
-- Considerind the subtle clue that Taylor left the bakery way to late on the timespan of the thief, lets consider bruce as the guilty, so lets get bruce accompliance
SELECT
  people.name
FROM
  people
  JOIN (
    SELECT
      receiver AS r
    FROM
      phone_calls
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND duration <= 60
      AND caller = '(367) 555-5533'
  ) AS receiver ON people.phone_number = receiver.r;

-- +-------+
-- | name  |
-- +-------+
-- | Robin |
-- +-------+
-- Robin is the accompliance!
