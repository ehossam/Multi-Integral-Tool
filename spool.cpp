#include <bits/stdc++.h>

//strings section
#define WELCOME_MESSAGE "Welcome to The ENGR313 Program\nFor the function option enter 'f' or 'F'\nFor the datapoints option enter 'd' or 'D'\n"
#define FUNCTION_MESSAGE "Enter the internal function without specifying dx or dy or ...etc:\n"
#define UNKOWN_INPUT_MESSAGE "uknown choice\n"
#define FUNCTION 'f'
#define DATAPOINTS 'd'

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
	/*cout << "Trap: "; */
	//std::cout << f0 << " " << f1 << "\n";
	return h*(f0 + f1) / 2.0;
}

double Simp13(double h, double f0, double f1, double f2)
{
	/*cout << "Simp13: ";
	cout << f0 << " " << f1 << " " << f2 << endl;*/
	return 2.0*h*(f0 + 4 * f1 + f2) / 6.0;
}

double Simp38(double h, double f0, double f1, double f2, double f3)
{
	/*cout << "Simp38: ";
	cout << f0 << " " << f1 << " " << f2 << " " << f3 << endl;*/
	return 3.*h*(f0 + 3 * (f1 + f2) + f3) / 8.0;
}

double integrate(double step, std::vector<double>values)
{
    //std::cout<<step<<'\n';
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

//main interaction and logic handling
int main()
{
char user_choice;
std::cout<< WELCOME_MESSAGE;
std::cin>>user_choice;
user_choice=tolower(user_choice); //function or datapoints
std::map<char,std::vector<double>> variables_values; //use it to store variable, its all values tuples
std::map<char,int> variables_num_of_regions; //use it to store variables, its number of points tuples
std::map<char,std::vector<double>> variables_limits; //get limits of each domain
std::vector<std::vector<double>> vectors_of_vectors_variable_values;  //vector of vectors
//start logic based on input from user
if (user_choice == FUNCTION)
{
//we get here if the user chooses to enter a function
//we should be able to get any number of variables that user enters
std::string func;
std::cout<<FUNCTION_MESSAGE;
std::cin.ignore();
std::getline(std::cin,func);

srand(time(NULL));  //will use for randoization logic

//now get the name of each variable
for(int i=0; i<func.length(); i++)
{
func[i]=tolower(func[i]);

//get all the variabels in equation
if (int(func[i])>=97 && int(func[i])<=122) //betwwee a and z ascii
{
   variables_values.insert(std::pair< char,std::vector<double> >(func[i],std::vector<double>())); //insert the variable now
   variables_limits.insert(std::pair< char,std::vector<double> >(func[i],std::vector<double>())); //insert the variable now
    //we will keep it for the program to determine the number of points for each domain
    //however we keep it a must to take at least 2 data points of course (region num=1)
   variables_num_of_regions.insert(std::pair<char,int>(func[i],rand() % 3 + 1));
}
}

//we can know go over the variables and push their values according to the number of points
double starting_limit,end_limit; //used for getting limit values logic
for (auto it = variables_limits.cbegin(); it != variables_limits.cend(); ++it)
{
    std::cout <<"\nPlease enter the TWO limits sperated by space not commas for domain \t"<< it->first <<": ";
    std::cin>>starting_limit>>end_limit;
    if(starting_limit==end_limit)
    {
    //terminate
    std::cerr<<"Invalid Input\n";
    std::exit(EXIT_FAILURE);
    }
    variables_limits[it->first].push_back(starting_limit);
    variables_limits[it->first].push_back(end_limit);
}

int num_of_regions; //used for filling values logic
double step; //used for filling values logic
std::vector<char> variables_with_orders;
std::vector<double> steps;
std::vector<int> num_of_points;
//now get over both limits and num_of_regions maps to fill in the num values map
for (auto it = variables_values.cbegin(); it != variables_values.cend(); ++it)
{
starting_limit= variables_limits[it->first][0];  //start of integral
end_limit= variables_limits[it->first][1];  //end of integral
num_of_regions=variables_num_of_regions[it->first]; //num of regions for the domains

num_of_points.push_back(num_of_regions+1);

step= double(std::abs(end_limit-starting_limit))/double(num_of_regions); //calculate the step to generate datapoints
for(double j=starting_limit;j<=end_limit;j+=step)
{
    variables_values[it->first].push_back(j);
}
variables_with_orders.push_back(it->first);
steps.push_back(step);
vectors_of_vectors_variable_values.push_back(it->second); //push the vector after you finish
}

 std:: ostringstream func_temp;
 std::string func_s;
 std::vector<std::vector<double>> output;
 std::vector<double> outputTemp;
 //get all the possible combinations of all elements
 cart_product(output, outputTemp, vectors_of_vectors_variable_values.begin(), vectors_of_vectors_variable_values.end());
 std::vector<std::vector<double>>::iterator row;
 std::vector<double>::iterator col;
 std::vector<char> all_char;
std::vector<double> value_temp;
char temp_char;
int flag_found=0;

//we need to store the values for each pount in a domain at a vector of vectors
std::vector<std::vector<double>> evaluations_at_point;
int current_point=-1;
double result;

int num_of_variables=variables_with_orders.size();
int current_variable=num_of_variables-1;

int count_all=0;
int ia=0;
int count_private=0; //used to terminate the for loop

std::vector<std::vector<double>> copy_of_output=output;
std::vector<double> current_points; //have the current points to evaluate
std::vector<double> points_evaluated; //have the evaluation results at each iteration
std::vector<double > current_integrations; //will hold the result of simpson or trap to the calculated evals
int set_flag_out=0;
std::vector<double>to_evaluate;
std::vector<double>copy_of_integ;

//we just do all of the following to get integration over the very last two inner domains
    for(;;)
    {   //std::cout<<"I got there\n";
        if ((ia<num_of_points[current_variable]) && !(set_flag_out))
        {
            for(int ib=0;ib<num_of_variables;ib++)
            {
               // std::cout<<output[count_all][ib]<<"\t";
               current_points.push_back(output[count_all][ib]);
            }

            //now look for the values for each domain
            for(int ic=0;ic<func.length();ic++)
            {
                temp_char=func[ic];
                flag_found=0;
                for(int id=0;id<variables_with_orders.size();id++)
                {
                    if(variables_with_orders[id]==temp_char)
                    {
                        func_temp<<current_points[id]<<" ";
                        flag_found=1;
                        break;
                    }
                }
            if (!flag_found)
                {
            func_temp<<func[ic]<<" ";
                }
            }
            result=evaluate(func_temp.str());
            std::cout<<func_temp.str()<<"\t" <<result<<"\n";
            points_evaluated.push_back(result);
            func_temp.str("");
            current_points.clear();
            count_all++;
            count_private++;
            ia++;
        }
        else if(count_all>= output.size())
        {
         current_integrations.push_back(integrate(steps[current_variable],points_evaluated));
         std::cout<<"result =" << integrate(steps[current_variable],points_evaluated)<<"\n";
         set_flag_out=1;
         for(;;)
            {output.clear();
            current_variable--;

            if(current_variable<=-1)
            {
            break;
            }
            else
            {
                std::cout<<"\n\n\n";
                for (int ss=0;ss<current_integrations.size();ss+=num_of_points[current_variable])
                {
                    for(int bb=0;bb<num_of_points[current_variable];bb++)
                    {
                        //std::cout<<"\n"<<current_integrations[ss+bb]<<'\t';
                        to_evaluate.push_back(current_integrations[ss+bb]);
                        //output[ss].push_back(current_integrations[bb]);
                    }
                    copy_of_integ.push_back(integrate(steps[current_variable],to_evaluate));
                    to_evaluate.clear();
                }
                current_integrations.clear();
                current_integrations=copy_of_integ;
                //std::cout<<current_integrations.size()<<"\t";
                copy_of_integ.clear();
            }
        }
        break;
        }
        else
        {
            //evaluate the integral first over this domain
            current_integrations.push_back(integrate(steps[current_variable],points_evaluated));
            std::cout<<"result =" << integrate(steps[current_variable],points_evaluated)<<"\n";
            //for(int iop=0; iop<points_evaluated.size();iop++)
            //{
            //    std::cout<<points_evaluated[iop]<<"\t";
            //}
            std::cout<<"\n";
            points_evaluated.clear();
            ia=0; //repeat again
        }
    }


for(int i=0;i<current_integrations.size();i++)
{
    std::cout<<"\n\n\n"<<current_integrations[i]<<'\t';
}


}
else if(user_choice == DATAPOINTS){
std::cout<<"uai\n";
}

else{
std::cerr<< UNKOWN_INPUT_MESSAGE;
std::exit (EXIT_FAILURE);
}

return 0;
}
