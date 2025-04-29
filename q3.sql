USE HospitalDB;

SELECT
  dept.Dept_Name,
  ROUND(
    SUM(b.Room_Cost)
  + SUM(b.Test_Cost)
  + SUM(b.M_Cost), 2
  ) AS TotalRevenue
FROM Bill      b
JOIN Room     r   ON b.Patient_ID = r.Patient_ID
JOIN Doctor   doc ON r.Doctor_ID  = doc.Doctor_ID
JOIN Staff    s   ON doc.Emp_ID    = s.Emp_ID
JOIN Department dept ON s.Dept_ID = dept.Dept_ID
GROUP BY
  dept.Dept_Name
ORDER BY
  TotalRevenue DESC;


--Aggregates billing by the department of the attending doctor.