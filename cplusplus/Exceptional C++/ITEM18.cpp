ITEM18:´úÂëµÄ¸´ÔÓĞÔ
	//How many execution paths could there be in the following code?

		String EvaluateSalaryAndReturnName( Employee e ) 
		{
		  if( e.Title() == "CEO" || e.Salary() > 100000 )
		  {
		    cout << e.First() << " " << e.Last() << " is overpaid" << endl;
		  }
		  return e.First() + " " + e.Last();
		}

