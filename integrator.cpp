#include "integrator.h"
#include <bits/stdc++.h>
#include <fstream>

Integrator::Integrator()
{

}

double averageSurfaceArea(std::vector<double>a)
{
    int size = a.size();
    double avg;
    double sum = 0.0;
    for (int i = 0; i < size; i++)
        sum += a[i];

    avg = sum / size;
    return avg;
}

double Trapozide(double h, double f0, double f1)
{
    return h*(f0 + f1) / 2.0;
}

double Simp13(double h, double f0, double f1, double f2)
{
    return 2.0*h*(f0 + 4 * f1 + f2) / 6.0;
}

double Simp38(double h, double f0, double f1, double f2, double f3)
{
    return 3.*h*(f0 + 3 * (f1 + f2) + f3) / 8.0;
}

double integrate(double step, std::vector<double>values)
{
    //aply trappzidal
    //std::cout<< "\nsize is  "<<values.size()<<"\n";
    if(values.size()==2)
    {
        return Trapozide(step,values[0],values[1]);
    }
    //apply simpson13
    else if(values.size()==3)
    {
        return Simp13(step,values[0],values[1],values[2]);
    }
    //apply simpson38
    else if(values.size()==4)
    {
        return Simp38(step,values[0],values[1],values[2],values[3]);
    }
    //composite simpson
    else
    {
        double sum=0;
        //if the number of points is even then apply both 12 and 38 but otherewise all should be 13
        if(values.size()%2==0)
        {
            for (int i=2;i<values.size();i+=2)
            {
                sum+=Simp13(step,values[i-2],values[i-1],values[i]);
            }
        }
        else
        {   int i;
            for ( i=2;i<values.size()-1;i+=2)
            {
                sum+=Simp13(step,values[i-2],values[i-1],values[i]);
            }
            i++;
            //now apply the simpson 3/8 for last 4 points
            sum+=Simp38(step,values[i-3],values[i-2],values[i-1],values[i]);
        }
    return sum;
    }
}

// Function to find precedence of
// operators.
int precedence(char op){
    if(op == '+'||op == '-')
        return 1;
    if(op == '*'||op == '/')
        return 2;
    if(op == '^')
        return 3;
    return 0;
}

// Function to perform arithmetic operations.
float applyOp(double a, double b, char op){
    switch(op){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return std::pow(a,b);
    }
}

// Function that returns value of
// expression after evaluation.
double evaluate(std::string tokens){
    int i;
    int flag_found_negative=1;

    //std::cout<<tokens<<"\n";

    // stack to store integer values.
    std::stack <double> values;

    // stack to store operators.
    std::stack <char> ops;

    //std::cout<<tokens[2]<<"\n";
    for(i = 0; i < tokens.length(); i++){

        //std::cout<<tokens[1]<<"\n";
        // Current token is a whitespace,
        // skip it.
        if(tokens[i] == ' ')
            continue;

        // Current token is an opening
        // brace, push it to 'ops'
        else if(tokens[i] == '('){
            ops.push(tokens[i]);
        }


        //maybe encountered negative number
        else if(tokens[i] == '-' && (isdigit(tokens[i+1])))
        {
            flag_found_negative=-1;

        }
        // Current token is a number, push
        // it to stack for numbers.
        else if(isdigit(tokens[i])){
            double val = 0;
            double domin=1;
            int flag_frac=0;
            // There may be more than one
            // digits in number. or there may be fraction point
            while(i < tokens.length() &&
                        (isdigit(tokens[i]) || tokens[i]=='.'))
            {
                if(isdigit(tokens[i]) && !flag_frac) {
                val = (val*10) + (tokens[i]-'0');
                i++;
                }
                else if(tokens[i] == '.')
                {
                flag_frac=1;
                i++;
                }
                else
                {
                domin*=10;
                val+=double(tokens[i]-'0')/double(domin);
                i++;
                }
            }
            val*=flag_found_negative;
            values.push(val);
            flag_found_negative=1;
        }

        // Closing brace encountered, solve
        // entire brace.
        else if(tokens[i] == ')')
        {
            while(!ops.empty() && ops.top() != '(')
            {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }

            // pop opening brace.
            ops.pop();
        }

        // Current token is an operator.
        else
        {
            // While top of 'ops' has same or greater
            // precedence to current token, which
            // is an operator. Apply operator on top
            // of 'ops' to top two elements in values stack.
            while(!ops.empty() && precedence(ops.top())
                                >= precedence(tokens[i])){
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }

            // Push current token to 'ops'.
            ops.push(tokens[i]);
        }
    }

    // Entire expression has been parsed at this
    // point, apply remaining ops to remaining
    // values.
    while(!ops.empty()){
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(val1, val2, op));
    }

    // Top of 'values' contains result, return it.
    return values.top();
}


// recursive algorithm to to produce cart. prod.
// At any given moment, "me" points to some Vi in the middle of the
// input data set.
//   for int i in *me:
//      add i to current result
//      recurse on next "me"
//
void cart_product(
    std::vector<std::vector<double>> & rvvi,  // final result
    std::vector<double> &  rvi,   // current result
    std::vector<std::vector<double>> ::const_iterator me, // current input
    std::vector<std::vector<double>>::const_iterator endv) // final input
{
    if(me == endv) {
        // terminal condition of the recursion. We no longer have
        // any input vectors to manipulate. Add the current result (rvi)
        // to the total set of results (rvvvi).
        rvvi.push_back(rvi);
        return;
    }

    // need an easy name for my vector-of-ints
    const std::vector<double>& mevi = *me;
    for(std::vector<double>::const_iterator it = mevi.begin();
        it != mevi.end();
        it++) {
        // final rvi will look like "a, b, c, ME, d, e, f"
        // At the moment, rvi already has "a, b, c"
        rvi.push_back(*it);  // add ME
        cart_product(rvvi, rvi, me+1, endv); //add "d, e, f"
        rvi.pop_back(); // clean ME off for next round
    }
}

//return result for a function input
std::string Integrator::getResultByFunction(std::string func,std::string limits_str)
{
    //all the following sub-section is for parsing the limits string passed in by ui
    //taking limits from user
    char current_var;
    //we can know go over the variables and push their values according to the number of points
    double starting_limit,end_limit; //used for getting limit values logic
    int flag_found_negative=1;
    int xx=0;
    double val = 0;
    double domin=1;
    int flag_frac=0;
    std::map<char,std::vector<double>> variables_limits; //get limits of each domain

    //get the limits of the string passed by ui
    //we also get to know the variables used from here
    for (;;)
    {
      //we need to break the loop when reaching the limit of string len
      if(xx>=limits_str.length()){
          variables_limits[current_var].push_back(starting_limit);
          variables_limits[current_var].push_back(end_limit);
          break;
      }
      //when we find a character betweemn a and z, we store it as the current variable and then
      //we enter a loop that iterates over the following digits, these all should be the starting limit for
      //such variable
      else if (int(limits_str[xx])>=97 && int(limits_str[xx])<=122) //betwween a and z ascii
      {
          current_var=tolower(limits_str[xx]);
          variables_limits.insert(std::pair< char,std::vector<double> >
                                  (current_var,std::vector<double>())); //make new place for this variable in the vector
          val = 0;
          domin=1;
          flag_frac=0;
          xx++;
          //we need to check for the negative sign
          if(limits_str[xx]=='-')
          {
              flag_found_negative=-1;
              xx++;
          }
          // There may be more than one
          // digits in number. or there may be fraction point
          while(xx < limits_str.length() && (isdigit(limits_str[xx]) || limits_str[xx]=='.'))
          {
           //get a many digit number
           if(isdigit(limits_str[xx]) && !flag_frac)
           {
            val = (val*10) + (limits_str[xx]-'0');
            xx++;
           }
           //get a fraction
           else if(limits_str[xx] == '.')
           {
            flag_frac=1;
            xx++;
           }
           //add the value of dominator
           else
           {
            domin*=10;
            val+=double(limits_str[xx]-'0')/double(domin);
            xx++;
           }
          }
          val*=flag_found_negative;
          starting_limit=val;
          flag_found_negative=1;
       }
      //we get into teh following condition when having the ending limit for the same current variable that we
      //got earlier from the previous condition
      else if(limits_str[xx]==',')
          {
            xx++;
            val = 0;
            domin=1;
            flag_frac=0;
            //we need to check for the negative sign
            if(limits_str[xx]=='-')
            {
                flag_found_negative=-1;
                xx++;
            }
            // There may be more than one
            // digits in number. or there may be fraction point
            while(xx < limits_str.length() && (isdigit(limits_str[xx]) || limits_str[xx]=='.'))
            {
              if(isdigit(limits_str[xx]) && !flag_frac)
              {
                val = (val*10) + (limits_str[xx]-'0');
                xx++;
              }
              else if(limits_str[xx] == '.')
              {
                flag_frac=1;
                xx++;
              }
              else
              {
                domin*=10;
                val+=double(limits_str[xx]-'0')/double(domin);
                xx++;
                }
             }
             val*=flag_found_negative;
             end_limit=val;
             flag_found_negative=1;
            }
            else if (limits_str[xx]==';')
            {
              if(starting_limit==end_limit)
               {
                //terminate
                std::cerr<<"Invalid Input\n";
                std::exit(EXIT_FAILURE);
               }
             //std::cerr<<starting_limit<<end_limit;
             variables_limits[current_var].push_back(starting_limit);
             variables_limits[current_var].push_back(end_limit);
             xx++;
            }
    }

    //the following sub-section is to store the number of points,steps, and values taken by each domain variable
    double step; //used for filling values logic
    std::vector<char> variables_with_orders;
    std::vector<double> steps;
    std::vector<int> num_of_points;
    double randomresult;
    srand(time(NULL));  //will use for randoization logic in creating num of regions
    std::vector<double>current_variable_values; //vector to store values for current variable
    std::vector<std::vector<double>> vectors_of_vectors_variable_values;  //vector of vectors for allvealues

    //now get over both limits and num_of_regions maps to fill in the num values map
    for (auto it = variables_limits.cbegin(); it != variables_limits.cend(); ++it)
    {
    current_variable_values.clear(); //make sure that all values are empty for this domain first
    starting_limit= variables_limits[it->first][0];  //start of integral
    end_limit= variables_limits[it->first][1];  //end of integral
    randomresult=rand() % 3 + 2;
    num_of_points.push_back(randomresult);    //at least two points are added
    //in the next line we calculate the step to generate datapoint (note that num of points-1 is the n:number of regions) h=(b-a)/n
    step= double(std::abs(end_limit-starting_limit))/double(randomresult-1);
    variables_with_orders.push_back(it->first); //add the variables in order
    steps.push_back(step);  //add the steps in the same order as variables
    for(double j=starting_limit;j<=end_limit;j+=step)
    {
            current_variable_values.push_back(j);
    }
    //the next line pushes values taken by each domain in the same order as occurence of that domain
    vectors_of_vectors_variable_values.push_back(current_variable_values);
    }

    //the following sub-section gets all possible combinations of values for all domains together
    std:: ostringstream func_temp; //used to hold modified version of function for evaluation
    std::vector<std::vector<double>> output;   //used to hold the result of cartesian product of all vector variables
    std::vector<double> outputTemp; //used internally in the cartesian product function
    //the following line gets all the possible combinations of all elements
    cart_product(output, outputTemp, vectors_of_vectors_variable_values.begin(), vectors_of_vectors_variable_values.end());

    //The following sub-section is used to get the integration result
    //it goes over the function string and evaluates it at each combination of datapoints
    //such evaluations are requested based on the number of points for each domain
    char temp_char;
    int flag_found=0;
    double result; //stores the evaluation of the function at certain combinations of points
    int num_of_variables=variables_with_orders.size();
    int current_variable=num_of_variables-1; //we start with last variable points and evaluate its function points
    int count_all=0; //used to count all iterations over the cartesion product output
    int ia=0;   //used insied the for loop to know the index of current element
    int count_private=0; //used to terminate the for loop
    std::vector<double> current_points; //have the current points to evaluate
    std::vector<double> points_evaluated; //have the evaluation results at each iteration
    std::vector<double > current_integrations; //will hold the result of simpson or trap to the calculated evals
    int set_flag_out=0;
    std::vector<double>to_evaluate;
    std::vector<double>copy_of_integ;

    //we just do all of the following to get integration over the very last two inner domains
    for(;;)
    {
      //this condition is only satisifed if we can still iterate over the output vector
      if ((ia<num_of_points[current_variable]) && !(set_flag_out))
      {
        //change the values of only one variable which happens to be the very last variable in the vector
        for(int ib=0;ib<num_of_variables;ib++)
        {
          current_points.push_back(output[count_all][ib]); //push one of the combinations of all domain values
        }
        //now look for the values for each domain and evaluate the function at this combination points
        for(int ic=0;ic<func.length();ic++)
        {
          temp_char=func[ic];
          flag_found=0;
          for(int id=0;id<variables_with_orders.size();id++)
          {
              //the current character is a domain variable name (i.e. x,y,z,..etc)
              //then we concatenate its evaluation for now
              if(variables_with_orders[id]==temp_char)
              {
                  func_temp<<current_points[id]<<" ";
                  flag_found=1;
                  break;
              }
          }
          //The current character is a number, a negative sign or operator, then we just concatenate it
          if (!flag_found)
          {
              func_temp<<func[ic]<<" ";
          }
       }
        //now we are done and have the function string ready to get evaluated
        // we send it to our evaluator function and get back our fancy result
        result=evaluate(func_temp.str());
        points_evaluated.push_back(result); //just push back that result in the vector
        func_temp.str(""); //clear the function string
        current_points.clear(); //clear the current points vector
        count_all++; //increment the count of iteration over output vector by 1
        count_private++;
        ia++; //increment index by 1 and repeat the loop again
      }
      //else we finsished the first iteration over the output vector
      //and we no longer have to evaluate the function again
      //just trap the fucntion here and make eneter an infinite loop until we
      //evaluate the final integration result
      else if(count_all>= output.size())
      {
          //the next line pushes the integration of the last points calculated before
          //there was no other place to evaluate this before but here
          current_integrations.push_back(integrate(steps[current_variable],points_evaluated));
          set_flag_out=1; //make sure not to jump again and exit this trap
          for(;;)
          {
              current_variable--; //do the same thing for the next variable (actually the one before the one we just finsihed)
              if(current_variable<=-1) //the only way out is that we are done with all domain variables
              {
                  break;
              }
              else
              {
                  //the logic is that we now have a vector of previous integrations
                  //what we need to do is that we fix the variable before last current and
                  //calculate integral at for the current variable
                  //so we make a valuable for loop that iterates over the current integration evaluations (for element
                  //that we just finsihed) then start to do integration for this vector but according to the new element(variable)
                  for (int ss=0;ss<current_integrations.size();ss+=num_of_points[current_variable])
                  {
                      for(int bb=0;bb<num_of_points[current_variable];bb++)
                      {
                          //we have new values to evaluate integral for
                          to_evaluate.push_back(current_integrations[ss+bb]);
                      }
                      //just push the integration result insied a temp vector for now
                      copy_of_integ.push_back(integrate(steps[current_variable],to_evaluate));
                      to_evaluate.clear();
                  }
                  current_integrations.clear(); //clear the integration results for previous element
                  current_integrations=copy_of_integ;// copy paste the integration result for current element to use it for next or as final result
                  copy_of_integ.clear();//dont forget to clear the temp vector
              }
            }
            break; //make sure to break out of this jail
      }
      //or we are just in need of more evaluations but the variable before last changed :(
      else
      {
          //evaluate the integral first over this domain
          current_integrations.push_back(integrate(steps[current_variable],points_evaluated));
          points_evaluated.clear(); //make sure that evaluation of function at these combination is cleared first
          ia=0; //repeat again
      }
    }

    //dont forget to send back the result of all of this to the ui
    //the result should be contained insied the current integration vector which should be having only one element here
    std:: ostringstream result_of_integration;
    result_of_integration<<current_integrations[0];
    return (result_of_integration.str());
}

//return result for a datapoints input
std::string Integrator::getResultByFile(std::string filename)
{


    //the next sub-section is used for getting values from the file and store it in all_values as vector of vectors
    //also the name of variables with order is stored in variables vector
    std::ifstream file(filename);
    std::vector<std::string> variables;
    std::vector<double> line_values;
    std::vector<std::vector<double>> all_values;
    if (file.is_open()) {
        std::string line;
        std::string firstword;
        int countlines=0;
        while (std::getline(file, line)) {
            firstword=line.c_str();
            std::istringstream iss(firstword);
            if (countlines==0)
            {
                countlines++;
                //we need to store the variable names at the very beginning
                for(std::string firstword; iss >> firstword; )
                    variables.push_back(firstword);
            }
            else{
                line_values.clear();
                //here we store the values taken by each variable along with the function evaluation at such evaluation
                for(std::string firstword; iss >> firstword; )
                    line_values.push_back(std::stod(firstword));
                all_values.push_back(line_values);
            }
        }
        //we need to get rid of the last element of the variable names because it just indicates the output name
        variables.erase(variables.end() - 1); // trying to erase the last element of vec
        //we need to close the file by now
        file.close();
    }

    //The next sub-section is used to iterate over the vector of vector and see the changes in the last variable first
    //and check the step in each line and make sure to apply appropriate integration
    double old_step=-1; //this has the step for the currently being processed value and the previous
    double new_step=-1;
    int current_variable=variables.size()-1; //this should conatin the current variable being processed
    int index_of_func_eval=variables.size(); //this has the index of the evaluation of the function at each combination
    std::vector<double> to_evaluate; //the values to evaluate their integration
    std::vector<double> current_integration_evaluations; //the evaluation of previous points
    std::vector<double> copy_of_integrations; //serves as a temp vector for integrations

    for(int ii=0;ii<all_values.size();ii++)
    {
        //push the evaluations of the function by now
        current_integration_evaluations.push_back(all_values[ii][index_of_func_eval]);
    }

    //for the very first element
    to_evaluate.push_back(current_integration_evaluations[0]);
    old_step=all_values[1][current_variable]-all_values[0][current_variable];
    int repetition_of_variable=0;
    int k=1;
    int set_flag=1;
    double sum_of_integration_region=0;
    int calc_step_flag=0;


    //do this for the last domain variable only
    while(current_variable-1>=0)
    {
      if(all_values[k][current_variable]!=all_values[k-1][current_variable]
            && all_values[k][current_variable-1]==all_values[k-1][current_variable-1])
            {
                set_flag=0;
                repetition_of_variable++;
                //check step
                new_step=all_values[k][current_variable]-all_values[k-1][current_variable];
                if(calc_step_flag)
                {
                    old_step=new_step;
                    calc_step_flag=0;
                }
                if(old_step==new_step)
                {
                    to_evaluate.push_back(current_integration_evaluations[repetition_of_variable]);
                }
                else
                {
                    sum_of_integration_region+=(integrate(old_step,to_evaluate));
                    to_evaluate.clear();
                    old_step=new_step;
                    to_evaluate.push_back(current_integration_evaluations[repetition_of_variable-1]);
                    to_evaluate.push_back(current_integration_evaluations[repetition_of_variable]);
                }
                k++;
            }
        else if (all_values[k][current_variable-1]!=all_values[k-1][current_variable-1])
            {
                repetition_of_variable++;
                sum_of_integration_region+=(integrate(old_step,to_evaluate));
                to_evaluate.clear();
                to_evaluate.push_back(current_integration_evaluations[repetition_of_variable]);
                copy_of_integrations.push_back(sum_of_integration_region);
                sum_of_integration_region=0;
                k++;
                set_flag=1;
                calc_step_flag=1;
            }
      else{
          k++;
      }
            if(k>=all_values.size() && !set_flag)
            {
                set_flag=0;
                repetition_of_variable=0;
                sum_of_integration_region+=(integrate(old_step,to_evaluate));
                to_evaluate.clear();
                copy_of_integrations.push_back(sum_of_integration_region);
                current_integration_evaluations.clear();
                current_integration_evaluations=copy_of_integrations;
                to_evaluate.push_back(current_integration_evaluations[0]);
                //std::cerr<<current_integration_evaluations[0];
                copy_of_integrations.clear();
                sum_of_integration_region=0;
                calc_step_flag=1;
                k=1;
                current_variable--;
            }
    }


    //final iteration is for the final values to get the current steps
    old_step=-1;
    new_step=-1;
    copy_of_integrations.clear();
    repetition_of_variable=0;
    sum_of_integration_region=0;
    to_evaluate.clear();
    to_evaluate.push_back(current_integration_evaluations[0]);

    for (int ys=0;ys<current_integration_evaluations.size();ys++)
        std::cerr<<current_integration_evaluations[ys]<<"\n";
    //check steps in the very first domain variable
    for(int y=1;y< all_values.size();y++)
    {
        if(all_values[y][0]!=all_values[y-1][0])
        {
            repetition_of_variable++;
            new_step=all_values[y][0]-all_values[y-1][0];
            if((new_step!=old_step) &&repetition_of_variable!=1)
            {
                sum_of_integration_region+=integrate(old_step,to_evaluate);
                to_evaluate.clear();
                old_step=new_step;
                to_evaluate.push_back(current_integration_evaluations[repetition_of_variable-1]);
                to_evaluate.push_back(current_integration_evaluations[repetition_of_variable]);
            }
            else
            {
                to_evaluate.push_back(current_integration_evaluations[repetition_of_variable]);
                old_step=new_step;
            }
        }
    }
    sum_of_integration_region+=integrate(old_step,to_evaluate);
    std:: ostringstream result_of_integration;
    result_of_integration<<sum_of_integration_region;
    return (result_of_integration.str());
}
