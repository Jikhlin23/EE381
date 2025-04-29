USE HospitalDB;

SELECT
  i.Provider_Name,
  COUNT(DISTINCT b.Patient_ID) AS PatientsCovered
FROM Bill      b
JOIN Insurance i ON b.Policy_Number = i.Policy_Number
WHERE (b.Total - b.Remaining_Balance) / b.Total >= 0.80
GROUP BY
  i.Provider_Name
ORDER BY
  PatientsCovered DESC;


--Shows which insurers are most “used” (claims paid ≥ 80% of bill).