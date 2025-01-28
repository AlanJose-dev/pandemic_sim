# Pandemic Sim

## What is

<p>
The <b>Pandemic Sim</b> is a program that aims to simulate the result of contamination of a viral disease in a population over a period of weeks.
</p>

## How it works

<p>
To carry out the simulation, the program uses the <a href="https://en.wikipedia.org/wiki/Random_walk">Random Walk</a>
algorithm to change states among individuals in a population, whose quantity is defined by the user, these states are:
</p>

<ul>
  <li>Healthy</li>
  <li>Isolated</li>
  <li>Sick</li>
  <li>Dead</li>
  <li>Immune</li>
</ul>

<p>
There is a transition probability for each state, that is, the chance that an individual
going from healthy to sick, from sick to dead and so on. To the
probabilities are shown in the table below:
</p>

<table style="background-color: rgba(0, 0, 0, .2); border-radius: .5rem">
  <thead>
    <tr>
      <td>&nbsp;</td>
      <td>Healthy</td>
      <td>Isolated</td>
      <td>Sick</td>
      <td>Dead</td>
      <td>Immune</td>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Healthy</td>
      <td style="text-align: end;">0.62</td>
      <td style="text-align: end;">0.3</td>
      <td style="text-align: end;">0.05</td>
      <td style="text-align: end;">0.0</td>
      <td style="text-align: end;">0.03</td>
    </tr>
    <tr>
      <td>Isolated</td>
      <td style="text-align: end;">0.05</td>
      <td style="text-align: end;">0.64</td>
      <td style="text-align: end;">0.1</td>
      <td style="text-align: end;">0.01</td>
      <td style="text-align: end;">0.2</td>
    </tr>
    <tr>
      <td>Sick</td>
      <td style="text-align: end;">0.0</td>
      <td style="text-align: end;">0.1</td>
      <td style="text-align: end;">0.65</td>
      <td style="text-align: end;">0.1</td>
      <td style="text-align: end;">0.15</td>
    </tr>
    <tr>
      <td>Dead</td>
      <td style="text-align: end;">0.0</td>
      <td style="text-align: end;">0.0</td>
      <td style="text-align: end;">0.0</td>
      <td style="text-align: end;">1.0</td>
      <td style="text-align: end;">0.0</td>
    </tr>
    <tr>
      <td>Immune</td>
      <td style="text-align: end;">0.0</td>
      <td style="text-align: end;">0.05</td>
      <td style="text-align: end;">0.02</td>
      <td style="text-align: end;">0.0</td>
      <td style="text-align: end;">0.93</td>
    </tr>
  </tbody>
</table>

## Inside Random Walk

<p>
When the simulation starts, the population grids are filled with individuals
defined as healthy, and then a sick individual is placed in the center
of each grid. From this individual, transition probabilities are calculated 
individual and social interactions. If the distancing effect is applied, the
contagion factor will be cumulatively reduced according to the number of individuals
in the isolated state increases, and then resets to the default value. For each calculation of
interaction carried out, a random number is generated and then compared to the sum of the
transition probabilities, and if a condition is satisfied, the individual will have their state
changed to another corresponding or random one. At the end of the iterations, the quantity
of individuals who have the status required in the execution.
</p>

## How to use

<p>
<b>Pandemic Sim</b> is a <i>CLI</i> program, which receives parameters for configuring the simulation. To run the program, simply call the <i>simulator</i> executable.
</p>

<code>.\simulator.exe -r &lt;value&gt; -p &lt;value&gt; -g &lt;value&gt; -c &lt;value&gt; -s -t &lt;value&gt; -o &lt;value&gt; -i</code>

<hr>

#### Parameters explained

<div style="background-color: rgba(0, 0, 0, .2); padding: 1rem; border-radius: .5rem;">

#### -r | --runs

<p>
Specifies the number of executions (results) you want to obtain. The default value is 1000.
</p>

#### -p | --population

<p>
Defines the size of the target population matrix, as it is a square, only one side is needed, e.g. 50 (50 * 50). The default value is 100.
</p>

#### -g | --generations

<p>
Defines the duration of each execution, for the interpretation of the results, it is understood as weeks. The default value is 52 (one year).
</p>

#### -c | --contagion-factor

<p>
It defines the contagion factor of the disease, which is a metric that informs its probability of transmission. It is given by values ​​between 0.1 and 1. Any value below or above this range will cause an exception <a href="https://cplusplus.com/reference/stdexcept/out_of_range/"><i>out_of_range</i></a>. The default value is 0.5.
</p>

#### -s | --social-distance-effect

<p>
Enables the effect of social distancing/lockdown in the simulation, with this feature active, the contagion factor will suffer a cumulative reduction based on the number of individuals in the <i>isolated</i> state, and at the end of the execution it will be reset to the value standard. This parameter requires no values.
</p>

#### -t | --threads

<p>
The program supports <a href="https://blog.tecnospeed.com.br/o-que-e-multithreading-e-como-a-tecnica-beneficia-seu-software/">multithreading</a>, but it uses only one thread by default. If the machine has more threads and you wish to use them to distribute the processing load, simply inform the quantity to be used through this parameter. The maximum value is the number of threads present on the current CPU. If a value 0 or greater than the available quantity is passed, it will cause an <i>out_of_range</i> exception. To check the number of threads available, simply consult the processor information or use the <code>-h</code> option, the information will be available there.
</p>

#### -o | --output-state

<p>
This parameter informs the state of the number of individuals that should be displayed at the end of each simulation.
<ul>
  <li><b style="color: green;">Healthy</b>: 0</li>
  <li><b style="color: black;">Isolated</b>: 1</li>
  <li><b style="color: yellow;">Sick</b>: 2</li>
  <li><b style="color: red;">Dead</b>: 3</li>
  <li><b style="color: blue;">Immune</b>: 4</li>
</ul>

#### -i | --image

<p>
If this option is present, the program will generate an image as a visual example of the dispersion of individuals with each state represented by a color.
<ul>
  <li><b style="color: green;">Green</b> (Healthy)</li>
  <li><b style="color: black;">Black</b> (Isolated)</li>
  <li><b style="color: yellow;">Yellow</b> (Sick)</li>
  <li><b style="color: red;">Red</b> (Dead)</li>
  <li><b style="color: blue;">Blue</b> (Immune)</li>
</ul>

#### -v | --version

<p>
Displays version and system information.
</p>

#### -h | --help

<p>
Displays a help message with an explanation of the parameters.
</p>
</div>
