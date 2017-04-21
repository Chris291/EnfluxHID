/* Copyright (c) 2017 Enflux Inc.
 * By downloading, accessing or using this SDK, you signify that you have read,
 * understood and agree to the terms and conditions of the End User License Agreement
 * located at: https://www.getenflux.com/pages/sdk-eula
 */

// PullInterfaceApp.cpp : Uses the Enflux pull interface to control an Enflux device
//

#include "stdio.h"

#include "ParseEnfluxTypes.h"
#include "EnfluxPullInterface.h"
#include "Recording.h"

// Change this to a valid file name on your PC
char* recording_file_name = "D:\\TestLog.enfl";

int main()
{
    devices devs = devices_shirt;
    // Starts streaming the filtered data for the shirt.
    StartStreamingPull(devs);
    enfl_quat_t rotations[ENFL_SENSOR_COUNT];
    int quit = 0;
    while (!quit)
    {
        // Input a one character command for each operation.
        // Press enter to send the command.
        int ch = getchar();
        switch (ch)
        {
        case 'q': //quit
            quit = 1;
            break;
        case 'r': // Read input commands in the order they are received.
            if (HasNewCommand(devs))
            {
                int command = PopCommand(devs);
                printf("Status received: %d\n", command);
            }
            else
            {
                printf("No new status");
            }
            break;
        case 'p': // Pull the latest positions

            LoadRotations(devs, rotations);
            // Prints the quaternion as {W,X,Y,Z}
            float center_sensor[4] = UNPACK_ENFL_QUAT_T(rotations[S_CENTER]);
            // Prints out the quaternion in the format {w,x,y,z}
            printf("New Positon. Center sensor is {%.02f %.02f %.02f %.02f}",
                center_sensor[0], center_sensor[1], center_sensor[2], center_sensor[3]);
            break;
        case 'c': // Start calibration
            StartCalibrationPull(devs);
            break;
        case 'e': // End streaming
            EndStreamingThread();
            break;
        case 's': // start streaming
            StartStreamingPull(devs);
            break;
        case 'j': // Begin recording
            int rec_result = StartRecording(recording_file_name);
            if (rec_result != REC_SUCCESS)
            {
                printf("Recording Error: %d\n", rec_result);
            }
            break;
        case 'k': // End recording
            int end_rec_result = EndRecording();
            if (end_rec_result != REC_SUCCESS)
            {
                printf("End recording Error: %d\n", end_rec_result);
            }
        }
    }
    // Finish the app by ending streaming, otherwise the device will continue to stream data to the PC.
    EndStreamingThread();
    return 0;
}