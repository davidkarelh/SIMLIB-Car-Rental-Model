# SIMLIB Car Rental Model
## Problem Description
The problem for this SIMLIB model was taken from the Simulation Modeling and Analysis textbook by Averill M. Law, namely from problem number 2.38. In general, a SIMLIB program is needed to simulate a bus trip through 3 places along with dropping off and picking up passengers at these places. Here is the problem:

![Problem 2.38 Part 1](https://github.com/davidkarelh/Tugas-Besar-1-Pemodelan-dan-Simulasi/blob/main/images/Problem-1.png?raw=true)
![Problem 2.38 Part 2](https://github.com/davidkarelh/Tugas-Besar-1-Pemodelan-dan-Simulasi/blob/main/images/Problem-2.png?raw=true)

## Events in Program
The program created has several events, namely:
- <b>EVENT_BUS_ARRIVAL             </b>: Event type for bus arrival.
- <b>EVENT_PEOPLE_ARRIVAL          </b>: Event type for people arrival.
- <b>EVENT_BUS_ALREADY_5_MINUTES   </b>: Event type for the 5 minutes mark of a bus in a location.
- <b>EVENT_PEOPLE_ENTER_BUS        </b>: Event type for people entering in a bus.
- <b>EVENT_PEOPLE_LEAVE_BUS        </b>: Event type for people leaving the bus.
- <b>EVENT_END_SIMULATION          </b>: Event type for end of the simulation.

## Event Handlers
The event handlers created to handle events in the program are:
- <b>void people_arrive(void)</b>:
The event being handled is EVENT_PEOPLE_ARRIVAL. This function will get the location of the person arriving and his destination from the transfer array. The program will put this person into the queue at the location where the person arrived. The program will then determine which list on the bus is right for this person (the queue of people on the bus is represented by 3 separate lists, each list is filled with people with different destinations). The program will then find out whether the bus is currently at this location or not. If the bus is currently at this location and the number of people on the bus is less than 20 and there are no people in the loading zone, the person will be removed from the location queue and put into the loading zone, then EVENT_PEOPLE_ENTER_BUS will be scheduled to remove the person from the loading zone to be put into the bus in the future. The program will then schedule EVENT_PEOPLE_ARRIVAL for the next arrival of the person at that location in the future.

- <b>void bus_arrive(void)</b>:
The event that is handled is EVENT_BUS_ARRIVAL. This function gets the bus arrival location from the transfer array. The program will then determine which list in the bus is appropriate for that location (the list of people who want to get off at this location). If there are no people who want to get off at this location and the number of people on the bus is equal to 20, the program will immediately dispatch the bus to the next location (the bus will be marked as being on the way and EVENT_BUS_ARRIVAL will be scheduled in the future for the next location). If there are people on the bus who want to exit the bus to that location or people at that location who want to board the bus, the program will record the location and arrival time of the bus and schedule EVENT_BUS_ALREADY_5_MINUTES 5 minutes in the future. If there is a person who wants to get off the bus to that location, the program will remove the person from the bus and put them into the unloading zone and schedule EVENT_PEOPLE_LEAVE_BUS in the future to remove the person from the unloading zone. If there is no person who wants to exit the bus but there is a person who wants to board the bus from that location, the program will move the person to the loading zone and schedule a future EVENT_PEOPLE_ENTER_BUS to remove the person from the loading zone and enter the bus.

- <b>void people_enter_bus(void)</b>:
The event handled is EVENT_PEOPLE_ENTER_BUS. This function will get the bus location from the transfer array. The program will then determine which list on the bus is appropriate for the person and remove the person from the loading zone and insert them into the list on the appropriate bus. If the number of people on the bus has reached 20 or the bus has been at the location for more than 5 minutes and there are no other people who want to board the bus from the same location, the program will dispatch the bus to the next location (the bus will be marked as being on the road and EVENT_BUS_ARRIVAL will be scheduled in the future for the next location). If the number of people on the bus is less than 20 and there is another person in the queue at the same location who wants to board the bus, the program will move the person to the loading zone and schedule a future EVENT_PEOPLE_ENTER_BUS to remove the person from the loading zone and put them on the bus.

- <b>void people_leave_bus(void)</b>:
The event handled is EVENT_PEOPLE_LEAVE_BUS. This function will get the bus location from the transfer array. The program will then remove the person from the unloading zone. The program will then determine which list on the bus is appropriate for that location (the list of people who want to get off at that location). If there are people who want to get off the bus at this location, the program will remove the person from the bus and put them into the unloading zone and schedule a future EVENT_PEOPLE_LEAVE_BUS to remove the person from the unloading zone. If no other person wants to get off the bus at this location, the program will start the process of getting the person in the queue at this location onto the bus by moving the person to the loading zone and scheduling a future EVENT_PEOPLE_ENTER_BUS to get the person off the loading zone and onto the bus.

- <b>void bus_5_minues(void)</b>:
Theevent handled is EVENT_BUS_ALREADY_5_MINUTES. This function will get the bus location from the transfer array. The program will then check if the foam is still at that location or not. If the bus is still at that location and there is no one in the loading or unloading zone (no one wants to get off the bus or board the bus at that location), the program will dispatch the bus to the next location (the bus will be marked as being on the way and EVENT_BUS_ARRIVAL will be scheduled in the future for the next location).

 
- <b>void report(void)</b>:
The event that is handled is EVENT_SIMULATION_END. This function will write the output of the program to the specified output file. The program output consists of:
    - The average and maximum number of people in the queue at each location.
    - The average and maximum delay of people in the queue at each location.
    - The average and maximum number of people on the bus.
    - The average, maximum, and minimum of the bus time at each location.
    - The average, maximum, and minimum of the time the bus makes one round (departure from the rental car until departure from the next rental car).
    - The average, maximum, and minimum number of people in the system based on where the person arrived.
 
## Main Algorithm Outline
Broadly speaking, the program will execute the following algorithm:
1. The program will open the output file and write down the file title and time explanation in seconds,
2. The program calls init_simlib() and sets maxatr to 4.
3.  program will call the init_model() function. In the init_model() function, the following will be done:
    1. Define the probability distribution from location 3 to destination 1 to be 0.583 and to destination 2 to be 0.417.
    2. Departing the bus to location 1, the bus will be marked as being on the road and EVENT_BUS_ARRIVAL will be scheduled in t   he future (the length of time to travel from location 3 to location 1 to make it appear as if the bus started departing from location 3 to location 1) for location 1.
    3. Scheduling the first EVENT_PEOPLE_ARRIVE for all three locations in the system based on their respective time exponentials.
    4. Scheduling EVENT_END_SIMULATION at the 80th hour (since the system uses seconds for time, EVENT_END_SIMULATION is scheduled at the 288,000th second (8x3600)).
4. The program will perform the main loop of the program. Each time one loop is performed, the program will call the timing function to get the next event and call the appropriate event handler for that event. The loop will continue until EVENT_END_SIMULATION has been handled.
5. The program will close the output file.
 
## Notes
- Even though the problem states "The bus always spend 5 minutes at each location.", the maker of this program created the program such as the program will immediately dispatch the bus to the next location if the number of people in the bus is already full (contains 20 people).
- This program is created for the course "IF4021 Modeling and Simulation" at Institut Teknologi Bandung.