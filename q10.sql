USE HospitalDB;

SELECT
  p.Patient_ID,
  p.Patient_FName,
  p.Patient_LName,
  COUNT(*) AS Visits
FROM Appointment ap
JOIN Patient p
  ON ap.Patient_ID = p.Patient_ID
GROUP BY
  p.Patient_ID,
  p.Patient_FName,
  p.Patient_LName
ORDER BY
  Visits DESC
LIMIT 3;
--Shows your most frequent patients, useful for loyalty or chronic-care programs.

