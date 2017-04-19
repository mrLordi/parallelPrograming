-----------------------------------------------
-----   Lab 1. Ada. Semaphore             -----
-----   A = max(Z) * B + z * (MO * MT)    -----
-----   Denys Vodotiiets                  -----
-----   IP-42                             -----
-----   22.03.17                          -----
-----------------------------------------------


with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Synchronous_Task_Control; use Ada.Synchronous_Task_Control;

procedure Main is
   N: Integer := 6;
   P: Integer := 2;
   H: Integer := N/P;
   s0, s1, s2, s3, s4, s5: Suspension_Object;
   type Vector is array(1..N) of Integer;
   type Matrix is array(1..N) of Vector;
   a : Integer := Integer'First;
   Result, B, Z: Vector;
   MO, MT: Matrix;



   procedure inputVector (v : out vector) is
   begin
      for i in 1..N loop
         v(i) := 1;
      end loop;
   end inputVector;

   procedure outputVector (v : in vector) is
   begin
      if N > 6 then
         Put_Line("Vector is too large");
      else
         for i in 1..N loop
            Put(v(i));
            Put(" ");
         end loop;
         Put_Line("");
      end if;
   end outputVector;

   procedure inputMatrix (m : out Matrix) is
   begin
      for i in 1..N loop
         for j in 1..N loop
            m(i)(j) := 1;
         end loop;
      end loop;
   end inputMatrix;

   procedure run is
      task T1;
      task body T1 is
         MO1: Matrix;
         Z1, Temp: Vector;
         a1 : Integer := Integer'First;
         sum: Integer;
      begin
         Put_Line(">>T1 started");

         -- entering data
         inputVector(Z);

         -- send signal to T2 about end of input
         Set_True(s1);

         -- wait until T2 ends entering
         Suspend_Until_True(s2);

         -- copy mutual resources
         Suspend_Until_True(s0);
         Z1 := Z;
         MO1 := MO;
         Set_True(s0);

         -- calculating
         for i in 1..H loop
            if Z1(i) > a1 then
               a1 := Z1(i);
            end if;
         end loop;

         Suspend_Until_True(s0);
         if a1 > a then
            a := a1;
         end if;
         Set_True(s0);

         -- wait while T2 is calculating
         Suspend_Until_True(s3);

         -- send signal to T2 about end of calculating
         Set_True(s4);

          -- copy mutual resources
         Suspend_Until_True(s0);
         a1 := a;
         Set_True(s0);

         -- calculating
         for i in 1..H loop
            for j in 1..N loop
               sum := 0;
               for l in 1..N loop
                  sum := sum + MO1(l)(j) * MT(i)(l);
               end loop;
               Temp(j) := sum;
            end loop;

            sum := 0;
            for j in 1..N loop
               sum := sum + Temp(j) * Z1(j);
            end loop;

            Result(i) := a1 * B(i) + sum;
         end loop;

         -- wait while T2 is calculating
         Suspend_Until_True(s5);

         -- output result
         Put_Line("Result A is ");
         outputVector(Result);

         Put_Line(">>T1 finished");
      end T1;

      task T2;
      task body T2 is
         MO2: Matrix;
         Z2, Temp: Vector;
         a2 : Integer := Integer'First;
         sum: Integer;
      begin
         Put_Line(">>T2 started");

         -- entering data
         inputVector(B);
         inputMatrix(MO);
         inputMatrix(MT);

         -- send signal to T1 about end of input
         Set_True(s2);

         -- wait until T1 ends entering
         Suspend_Until_True(s1);

         -- copy mutual resources
         Suspend_Until_True(s0);
         Z2 := Z;
         MO2 := MO;
         Set_True(s0);

         -- calculating
         for i in H + 1..N loop
            if Z2(i) > a2 then
               a2 := Z2(i);
            end if;
         end loop;

         Suspend_Until_True(s0);
         if a2 > a then
            a := a2;
         end if;
         Set_True(s0);

         -- send signal to T1 about end of calculating
         Set_True(s3);

         -- wait while T1 is calculating
         Suspend_Until_True(s4);

          -- copy mutual resources
         Suspend_Until_True(s0);
         a2 := a;
         Set_True(s1);

         -- calculating
         for i in H+1..N loop
            for j in 1..N loop
               sum := 0;
               for l in 1..N loop
                  sum := sum + MO2(l)(j) * MT(i)(l);
               end loop;
               Temp(j) := sum;
            end loop;

            sum := 0;
            for j in 1..N loop
               sum := sum + Temp(j) * Z2(j);
            end loop;

            Result(i) := a2 * B(i) + sum;
         end loop;

          -- send signal to T1 about end of calculating
         Set_True(s5);

         Put_Line(">>T2 finished");
      end T2;

   begin
      null;
   end run;

begin
      Set_True(s0);
      run;
end Main;

