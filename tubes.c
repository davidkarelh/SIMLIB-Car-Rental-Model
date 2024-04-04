/* External definitions for car rental system using simlib. */

#include "simlib.h"             /* Required for use of simlib.c. */

#define EVENT_BUS_ARRIVAL        1  /* Event type for bus arrival. */
#define EVENT_PEOPLE_ARRIVAL      2  /* Event type for people arrival. */
#define EVENT_BUS_ALREADY_5_MINUTES  3	/* Event type for the 5 minutes mark of a bus in a location. */
#define EVENT_PEOPLE_ENTER_BUS  4	/* Event type for people entering in a bus. */
#define EVENT_PEOPLE_LEAVE_BUS  5	/* Event type for people leaving the bus. */
#define EVENT_END_SIMULATION  6	/* Event type for end of the simulation. */

#define LIST_LOCATION_1           1  /* List number for queue of location 1. */
#define LIST_LOCATION_2          2  /* List number for queue of location 2. */
#define LIST_LOCATION_3          3  /* List number for queue of location 3. */
#define LIST_BUS_1          4  /* List number for queue inside bus for passengers with destination 1. */
#define LIST_BUS_2          5  /* List number for queue inside bus for passengers with destination 2. */
#define LIST_BUS_3          6  /* List number for queue inside bus for passengers with destination 3. */
#define LIST_BUS_LOCATION          7  /* List number for list containing bus current's location. */
#define LIST_LOADING_OR_UNLOADING_ZONE          8  /* List number for list containing the unloading or loading zone for the passengers. */

#define STREAM_INTERARRIVAL_1   1	/* Random-number stream for interarrivals at location 1. */
#define STREAM_INTERARRIVAL_2   2	/* Random-number stream for interarrivals at location 2. */
#define STREAM_INTERARRIVAL_3   3	/* Random-number stream for interarrivals at location 3. */
#define STREAM_UNLOADING_TIMES       4	/* Random-number stream for unloading times. */
#define STREAM_LOADING_TIMES       5	/* Random-number stream for loading times. */
#define STREAM_DETERMINING_DESTINATION        6	/* Random-number stream for determining destination of an arrival at the car rental. */

#define BUS_DURATION_FROM_3_TO_1    540 /* Duration of the bus going from location 3 to location 1. Calculation: ((1 + 3 + 0.5) / 30) * 3600 */
#define BUS_DURATION_FROM_1_TO_2    120 /* Duration of the bus going from location 1 to location 2. Calculation: ((1) / 30) * 3600 */
#define BUS_DURATION_FROM_2_TO_3    540 /* Duration of the bus going from location 2 to location 3. Calculation: ((1 + 3 + 0.5) / 30) * 3600 */

#define MEAN_INTERARRIVAL_LOCATION_1 257.142857 /* Mean interarrival time of people in location 1. Calculation: (1 / 14) * 3600 */
#define MEAN_INTERARRIVAL_LOCATION_2 360.0 /* Mean interarrival time of people in location 3. Calculation: (1 / 10) * 3600 */
#define MEAN_INTERARRIVAL_LOCATION_3 150.0 /* Mean interarrival time of people in location 3. Calculation: (1 / 24) * 3600 */

#define SAMPST_DELAYS_LOCATION_1        1  /* sampst variable for delays in queue of location 1, denoted by number 1. */
#define SAMPST_DELAYS_LOCATION_2        2  /* sampst variable for delays in queue of location 2, denoted by number 2. */
#define SAMPST_DELAYS_LOCATION_3        3  /* sampst variable for delays in queue of location 3, denoted by number 3. */
#define SAMPST_TIME_THE_BUS_IS_STOPPED_AT_LOCATION_1        4  /* sampst variable for time the bus is stopped at location 1, denoted by number 1 + 3. */
#define SAMPST_TIME_THE_BUS_IS_STOPPED_AT_LOCATION_2        5  /* sampst variable for time the bus is stopped at location 2, denoted by number 2 + 3. */
#define SAMPST_TIME_THE_BUS_IS_STOPPED_AT_LOCATION_3        6  /* sampst variable for time the bus is stopped at location 3, denoted by number 3 + 3. */
#define SAMPST_TIME_A_PERSON_IS_IN_THE_SYSTEM_BY_ARRIVAL_LOCATION_1        7  /* sampst variable for time a person is in the system by arrival location 1, denoted by number 1 + 6. */
#define SAMPST_TIME_A_PERSON_IS_IN_THE_SYSTEM_BY_ARRIVAL_LOCATION_2        8  /* sampst variable for time a person is in the system by arrival location 2, denoted by number 2 + 6. */
#define SAMPST_TIME_A_PERSON_IS_IN_THE_SYSTEM_BY_ARRIVAL_LOCATION_3        9  /* sampst variable for time a person is in the system by arrival location 3, denoted by number 3 + 6. */
#define SAMPST_BUS_LOOP        10  /* sampst variable for time for the bus to make a loop (departure from the car rental to the next such departure). */

#define TIMEST_BUS_NUMBER        1  /* timest variable for time for bus capacity. */

/* Declare non-simlib global variables. */

double prob_distrib_destination[26], start_of_loop_time;
FILE *infile, *outfile;

/* Declare non-simlib functions. */

void init_model(void);
void people_arrive(void);
void bus_arrive(void);
void people_enter_bus(void);
void people_leave_bus(void);
void report(void);
void bus_5_minutes(void);
int number_of_people_in_bus(void);


int main()  /* Main function. */
{
    /* Open input and output files. */

    // infile  = fopen("tubes.in",  "r");
    outfile = fopen("tubes.out", "w");

    /* Write report heading and input parameters. */

    fprintf (outfile, "Car-rental Model\n\n");
    fprintf (outfile, "Time is in seconds\n\n");

    /* Initialize simlib */

    init_simlib ();

    /* Set maxatr = max(maximum number of attributes per record, 4) */

    maxatr = 4;			/* NEVER SET maxatr TO BE SMALLER THAN 4. */

    /* Initialize the model. */

    init_model();

    /* Run the simulation until it terminates after an end-simulation event
     (type EVENT_END_SIMULATION) occurs. */

    printf("TIME IS IN SECONDS\n");

    do {

        /* Determine the next event. */

        timing ();

        /* Invoke the appropriate event function. */

        // printf("%d\n", next_event_type);
        switch (next_event_type) {
            case EVENT_BUS_ARRIVAL:
                bus_arrive ();
                break;
            case EVENT_PEOPLE_ARRIVAL:
                people_arrive ();
                break;
            case EVENT_PEOPLE_ENTER_BUS:
                people_enter_bus();
                break;
            case EVENT_PEOPLE_LEAVE_BUS:
                people_leave_bus();
                break;
            case EVENT_BUS_ALREADY_5_MINUTES:
                bus_5_minutes();
                break;;
            case EVENT_END_SIMULATION:
                report ();
                break;
        }

        /* If the event just executed was not the end-simulation event (type
            EVENT_END_SIMULATION), continue simulating.  Otherwise, end the
            simulation. */
    } while (next_event_type != EVENT_END_SIMULATION);


    // fclose(infile);
    fclose(outfile);

    return 0;
}

void init_model(void)  /* Initialization function. */
{
    prob_distrib_destination[1] = 0.583;
    // prob_distrib_destination[2] = 0.417;
    prob_distrib_destination[2] = 1;
    start_of_loop_time = 0.0;

    /* Note defining attributes beyond the first two for the event record before invoking event_schedule. */

    /* Schedule the arrival of the bus from location 1 from location 3. */
    transfer[3] = 1; /* Define bus destination to location 1 */
    event_schedule (sim_time + BUS_DURATION_FROM_3_TO_1, EVENT_BUS_ARRIVAL);

    /* Schedule the arrival of the first person at location 1. */
    transfer[3] = 1; /* Define person's location of arrival */
    transfer[4] = 3; /* Define person destination to location 3 (air terminal) */
    event_schedule (sim_time + expon (MEAN_INTERARRIVAL_LOCATION_1, STREAM_INTERARRIVAL_1), EVENT_PEOPLE_ARRIVAL);

    /* Schedule the arrival of the first person at location 2. */
    transfer[3] = 2; /* Define person's location of arrival */
    transfer[4] = 3; /* Define person destination to location 3 (air terminal) */
    event_schedule (sim_time + expon (MEAN_INTERARRIVAL_LOCATION_2, STREAM_INTERARRIVAL_2), EVENT_PEOPLE_ARRIVAL);

    /* Schedule the arrival of the first person at location 3. */
    transfer[3] = 3; /* Define person's location of arrival */
    int destination = random_integer (prob_distrib_destination, STREAM_DETERMINING_DESTINATION);
    transfer[4] = destination; /* Define person destination to location */
    event_schedule (sim_time + expon (MEAN_INTERARRIVAL_LOCATION_3, STREAM_INTERARRIVAL_3), EVENT_PEOPLE_ARRIVAL);

      /* Schedule the end of the simulation.  (This is needed for consistency of
     units.) */

    event_schedule (80 * 3600, EVENT_END_SIMULATION);
}

void people_arrive(void)  /* Arrival event function. */
{
    int arrival_location = (int)transfer[3];
    int destination = (int)transfer[4];

    int list_bus = LIST_BUS_1;

    if (arrival_location == 2) {
        list_bus = LIST_BUS_2;
    } else if (arrival_location == 3) { /* arrival_location == 3 */
        list_bus = LIST_BUS_3;
    }

    int bus_location = 0;
    if (list_size[LIST_BUS_LOCATION] == 1) {
        list_remove(FIRST, LIST_BUS_LOCATION);
        bus_location = (int)transfer[1];
        list_file(LAST, LIST_BUS_LOCATION);
    }

    /* Insert person's arrival location to location's queue */
    transfer[1] = arrival_location;

    /* Insert person's destination to location's queue */
    transfer[2] = destination;

    /* Person's arrival time in the system */
    transfer[3] = sim_time;

    list_file(LAST, arrival_location);
    printf("TIME: %f \tPEOPLE ARRIVE AT STATION %d, DESTINATION %d, QUEUE LENGTH = %d\n", sim_time, arrival_location, destination, list_size[arrival_location]);

    if (list_size[arrival_location] == 1 && number_of_people_in_bus() < 20 && list_size[LIST_LOADING_OR_UNLOADING_ZONE] == 0) {
        if (bus_location == arrival_location) {
            list_remove(FIRST, bus_location);
            sampst(sim_time - transfer[3], bus_location);
            list_file(LAST, LIST_LOADING_OR_UNLOADING_ZONE);

            transfer[3] = bus_location;
            transfer[4] = list_bus;
            event_schedule (sim_time + uniform (15, 25, STREAM_LOADING_TIMES), EVENT_PEOPLE_ENTER_BUS);
        }

    }
    
    /* Schedule next person's arrival. */

    if (arrival_location == 1 || arrival_location == 2) {
        transfer[4] = 3;
        if (arrival_location == 1) {
            transfer[3] = 1;
            event_schedule (sim_time + expon (MEAN_INTERARRIVAL_LOCATION_1, STREAM_INTERARRIVAL_1), EVENT_PEOPLE_ARRIVAL);
        } else if (arrival_location == 2) { /* arrival_location == 2 */
            transfer[3] = 2;
            event_schedule (sim_time + expon (MEAN_INTERARRIVAL_LOCATION_2, STREAM_INTERARRIVAL_2), EVENT_PEOPLE_ARRIVAL);
        }
    } else { /* arrival_location == 3 */
        transfer[3] = 3;
        destination = random_integer (prob_distrib_destination, STREAM_DETERMINING_DESTINATION);
        transfer[4] = destination; /* Define person destination to location */
        event_schedule (sim_time + expon (MEAN_INTERARRIVAL_LOCATION_3, STREAM_INTERARRIVAL_3), EVENT_PEOPLE_ARRIVAL);
    }
}

void bus_arrive(void) {

    int bus_location = (int)transfer[3];
    printf("TIME: %f \tBUS ARRIVE AT STATION %d\n", sim_time, bus_location);
    int list_bus = LIST_BUS_1;

    if (bus_location == 2) {
        list_bus = LIST_BUS_2;
    } else if (bus_location == 3) { /* bus_location == 3 */
        list_bus = LIST_BUS_3;
    }

    if (list_size[list_bus] == 0 && number_of_people_in_bus() == 20) {
        int destination = 1; /* bus_location == 3 */
        int bus_duration = BUS_DURATION_FROM_3_TO_1;
        if (bus_location == 1) {
            destination = 2;
            bus_duration = BUS_DURATION_FROM_1_TO_2;
        } else if (bus_duration == 2) { /* bus_location == 2 */
            destination = 3;
            bus_duration = BUS_DURATION_FROM_2_TO_3;
        }

        if (destination == 1) {
            sampst(sim_time - start_of_loop_time, SAMPST_BUS_LOOP);
            start_of_loop_time = sim_time;
        }

        sampst(0.0, bus_location + 3);

        transfer[3] = destination;
        event_schedule (sim_time + bus_duration, EVENT_BUS_ARRIVAL);
        list_remove(FIRST, LIST_BUS_LOCATION);

    } else {
        transfer[1] = bus_location;
        transfer[2] = sim_time;

        list_file(FIRST, LIST_BUS_LOCATION);
        transfer[3] = bus_location;
        event_schedule (sim_time + 5 * 60, EVENT_BUS_ALREADY_5_MINUTES);
        
        if (list_size[list_bus] > 0) { /* There are people that want to drop off the bus */
            list_remove(FIRST, list_bus);

            timest(number_of_people_in_bus(), TIMEST_BUS_NUMBER);

            list_file(LAST, LIST_LOADING_OR_UNLOADING_ZONE);

            transfer[3] = bus_location;
            event_schedule(sim_time + uniform (16, 24, STREAM_UNLOADING_TIMES), EVENT_PEOPLE_LEAVE_BUS);

        } else if (list_size[bus_location] > 0) { /* There are no people that want to drop off the bus, but there are people that want to ride the bus */
            list_remove(FIRST, bus_location);
            sampst(sim_time - transfer[3], bus_location);
            list_file(LAST, LIST_LOADING_OR_UNLOADING_ZONE);

            transfer[3] = bus_location;

            event_schedule (sim_time + uniform (15, 25, STREAM_LOADING_TIMES), EVENT_PEOPLE_ENTER_BUS);

        }
    }
}

void people_enter_bus(void) {
    int bus_location = (int)transfer[3];
    int list_bus = LIST_BUS_1;

    list_remove(FIRST, LIST_LOADING_OR_UNLOADING_ZONE);

    int destination = (int)transfer[2];
    if (destination == 2) {
        list_bus = LIST_BUS_2;
    } else if (destination == 3) { /* destination == 3 */
        list_bus = LIST_BUS_3;
    }

    printf("TIME: %f \tPEOPLE ENTER BUS AT STATION %d, DESTINATION %d, QUEUE LENGTH = %d\n", sim_time, bus_location, (int)transfer[2], list_size[bus_location]);
    list_file(LAST, list_bus);

    timest(number_of_people_in_bus(), TIMEST_BUS_NUMBER);

    printf("\t\t\tNUMBER OF BUS PASSENGERS = %d, DEST 1 = %d, DEST 2 = %d, DEST 3 = %d\n", number_of_people_in_bus(), list_size[LIST_BUS_1], list_size[LIST_BUS_2], list_size[LIST_BUS_3]);

    list_remove(FIRST, LIST_BUS_LOCATION);
    double bus_arrival_time = transfer[2];
    list_file(LAST, LIST_BUS_LOCATION);
    
    if (number_of_people_in_bus() == 20 || (sim_time > (bus_arrival_time + 5 * 60) && list_size[bus_location] == 0)) {
        destination = 1; /* bus_location == 3 */
        int bus_duration = BUS_DURATION_FROM_3_TO_1;
        if (bus_location == 1) {
            destination = 2;
            bus_duration = BUS_DURATION_FROM_1_TO_2;
        } else if (bus_location == 2) { /* bus_location == 2 */
            destination = 3;
            bus_duration = BUS_DURATION_FROM_2_TO_3;
        }

        if (destination == 1) {
            sampst(sim_time - start_of_loop_time, SAMPST_BUS_LOOP);
            start_of_loop_time = sim_time;
        }

        list_remove(FIRST, LIST_BUS_LOCATION);
        sampst(sim_time - transfer[2], bus_location + 3);

        transfer[3] = destination;
        event_schedule (sim_time + bus_duration, EVENT_BUS_ARRIVAL);

    } else if (number_of_people_in_bus() < 20 && list_size[bus_location] > 0) {
        list_remove(FIRST, bus_location);
        sampst(sim_time - transfer[3], bus_location);
        list_file(LAST, LIST_LOADING_OR_UNLOADING_ZONE);

        transfer[3] = bus_location;
        event_schedule (sim_time + uniform (15, 25, STREAM_LOADING_TIMES), EVENT_PEOPLE_ENTER_BUS);
    }
}

void people_leave_bus(void) {
    int bus_location = (int)transfer[3];
    int list_bus = LIST_BUS_1;
    if (bus_location == 2) {
        list_bus = LIST_BUS_2;
    } else if (bus_location == 3) { /* bus_location == 3 */
        list_bus = LIST_BUS_3;
    }

    list_remove(FIRST, LIST_LOADING_OR_UNLOADING_ZONE);
    sampst(sim_time - transfer[3], (int)transfer[1] + 6);

    printf("TIME: %f \tPEOPLE LEAVE BUS AT STATION %d\n", sim_time, bus_location);
    printf("\t\t\tNUMBER OF BUS PASSENGERS = %d, DEST 1 = %d, DEST 2 = %d, DEST 3 = %d\n", number_of_people_in_bus(), list_size[LIST_BUS_1], list_size[LIST_BUS_2], list_size[LIST_BUS_3]);

    if (list_size[list_bus] > 0) {
        list_remove(FIRST, list_bus);
        list_file(LAST, LIST_LOADING_OR_UNLOADING_ZONE);

        timest(number_of_people_in_bus(), TIMEST_BUS_NUMBER);

        transfer[3] = bus_location;
        event_schedule(sim_time + uniform (16, 24, STREAM_UNLOADING_TIMES), EVENT_PEOPLE_LEAVE_BUS);
    } else {
        if (number_of_people_in_bus() < 20 && list_size[bus_location] > 0) {
            list_remove(FIRST, bus_location);
            list_file(LAST, LIST_LOADING_OR_UNLOADING_ZONE);

            transfer[3] = bus_location;
            event_schedule (sim_time + uniform (15, 25, STREAM_LOADING_TIMES), EVENT_PEOPLE_ENTER_BUS);
        }
    }   
}

void bus_5_minutes(void) {
    int bus_location = (int)transfer[3];

    int list_bus = LIST_BUS_1;
    if (bus_location == 2) {
        list_bus = LIST_BUS_2;
    } else if (bus_location == 3) { /* bus_location == 3 */
        list_bus = LIST_BUS_3;
    }

    if (list_size[LIST_BUS_LOCATION] > 0) { /* Check if bus is currently in a station */
        list_remove(FIRST, LIST_BUS_LOCATION);
        double bus_arrival_time = transfer[2];

        if (
            list_size[LIST_LOADING_OR_UNLOADING_ZONE] == 0
        ) {
            int destination = 1; /* bus_location == 3 */
            int bus_duration = BUS_DURATION_FROM_3_TO_1;
            if (bus_location == 1) {
                destination = 2;
                bus_duration = BUS_DURATION_FROM_1_TO_2;
            } else if (bus_location == 2) { /* bus_location == 2 */
                destination = 3;
                bus_duration = BUS_DURATION_FROM_2_TO_3;
            }

            if (destination == 1) {
                sampst(sim_time - start_of_loop_time, SAMPST_BUS_LOOP);
                start_of_loop_time = sim_time;
            }

            sampst(sim_time - bus_arrival_time, bus_location + 3);

            transfer[3] = destination;
            event_schedule (sim_time + bus_duration, EVENT_BUS_ARRIVAL);
        } else {
            transfer[1] = bus_location;
            transfer[2] = bus_arrival_time;
            list_file(LAST, LIST_BUS_LOCATION);
        }
    }

    printf("TIME: %f \t5 MINUTES BUS AT STATION %d, QUEUE LENGTH = %d, BUS QUEUE LENGTH = %d\n", sim_time, bus_location, list_size[bus_location], list_size[list_bus]);
}

int number_of_people_in_bus(void) {
    return list_size[LIST_BUS_1] + list_size[LIST_BUS_2] + list_size[LIST_BUS_3];
}

void report(void) {
    printf("END\n");

    fprintf(outfile, "Number in Each Queue\n");
    for (int i = 1; i <= 3; i++) {
        filest(i);
        fprintf(outfile, "\tLocation %d Queue\n", i);
        fprintf(outfile, "\t\tAverage\t%f\n", transfer[1]);
        fprintf(outfile, "\t\tMaximum\t%f\n\n", transfer[2]);
    }

    fprintf(outfile, "Delays in Each Queue\n");
    for (int i = 1; i <= 3; i++) {
        sampst(0.0, -1 * i);
        fprintf(outfile, "\tLocation %d Queue\n", i);
        fprintf(outfile, "\t\tAverage\t%f\n", transfer[1]);
        fprintf(outfile, "\t\tMaximum\t%f\n\n", transfer[3]);
    }

    timest(0.0, -1 * TIMEST_BUS_NUMBER);
    fprintf(outfile, "Number in Bus\n");
    fprintf(outfile, "\tAverage\t%f\n", transfer[1]);
    fprintf(outfile, "\tMaximum\t%f\n\n", transfer[2]);

    fprintf(outfile, "Time the Bus is Stopped at Each Location\n");
    for (int i = 1; i <= 3; i++) {
        int j = i + 3;
        sampst(0.0, -1 * j);
        fprintf(outfile, "\tLocation %d\n", i);
        fprintf(outfile, "\t\tAverage\t%f\n", transfer[1]);
        fprintf(outfile, "\t\tMaximum\t%f\n", transfer[3]);
        fprintf(outfile, "\t\tMinimum\t%f\n\n", transfer[4]);
    }

    fprintf(outfile, "Time for the Bus to Make a Loop (Departure From the Car Rental to the Next Such Departure)\n");
    sampst(0.0, -1 * SAMPST_BUS_LOOP);
    fprintf(outfile, "\tAverage\t%f\n", transfer[1]);
    fprintf(outfile, "\tMaximum\t%f\n", transfer[3]);
    fprintf(outfile, "\tMinimum\t%f\n\n", transfer[4]);

    fprintf(outfile, "Time a Person is in the System by Arrival Location\n");
    for (int i = 1; i <= 3; i++) {
        int j = i + 6;
        sampst(0.0, -1 * j);
        fprintf(outfile, "\tLocation %d\n", i);
        fprintf(outfile, "\t\tAverage\t%f\n", transfer[1]);
        fprintf(outfile, "\t\tMaximum\t%f\n", transfer[3]);
        fprintf(outfile, "\t\tMinimum\t%f\n\n", transfer[4]);
    }

}