USE HospitalDB;

SELECT
  s.Emp_ID,
  s.Emp_FName,
  s.Emp_LName,
  COUNT(*) AS TestsConducted
FROM Lab_Screening l
JOIN Staff s
  ON l.Technician_ID = s.Emp_ID
GROUP BY
  s.Emp_ID,
  s.Emp_FName,
  s.Emp_LName
ORDER BY
  TestsConducted DESC;


--Ranks lab technicians by test count.