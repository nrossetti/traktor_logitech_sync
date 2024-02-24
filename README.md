# Traktor Logitech Sync

## Project Overview
Traktor Logitech Sync is a project designed to enhance the DJing experience by integrating Logitech RGB keyboards with Traktor DJ software. This initiative, built upon the example code from the Logitech LED SDK, aims to provide DJs with a unique and immersive way to interact with their music. Key features include synchronizing to the waveform track ending warning (waveform flashes red), which flashes the keyboard's left hanf and right half red to alert DJs during live sessions, and dynamic lighting that corresponds to two tracks, lighting up the respective side of the keyboard for each track.

## Features
- **Track Ending Warning**: To prevent any surprises during a live set, the keyboard flashes red in the waveform area, signaling that a track is about to end.
- **Dynamic Lighting for Two Tracks**: The keyboard lighting responds to cues, loops, and beats from two tracks, with each side of the keyboard representing one track, enhancing the visual experience of mixing.
- **Customizable Zones**: Users can define zones on their keyboard to correspond with different functions or tracks within Traktor, tailoring the setup to their workflow.
- **Real-time Feedback**: Provides DJs with immediate visual feedback on their keyboard, reducing the need to constantly monitor the computer screen.

## Limitations
- **Compatibility**: Limited to Logitech keyboards with per-key RGB lighting supported by the Logitech LED SDK.
- **Software Requirements**: Traktor DJ software and Logitech Gaming Software/G HUB must be installed and operational.
- **Performance**: Responsiveness and performance may vary depending on system specs and lighting configuration complexity.
- **Window Positioning**: The application requires a fixed position and full visibility for the Traktor window due to reliance on pixel mapping for waveform cue detection.
- **Static Configuration**: Waveform cue pixel locations are hardcoded, necessitating manual adjustments for changes in Traktor window size or screen resolution.

## Setup
1. **Install Dependencies**: Ensure Traktor DJ software, Logitech Gaming Software/G HUB, and the Logitech LED SDK are installed on your system.
2. **Configuration**: Adjust the `keyzones.json` file to define your keyboard's lighting zones to match your Traktor setup preferences.
3. **Running the Application**: Launch the application, which will automatically sync with the Traktor window and begin the keyboard lighting synchronization with software events.

Make sure the application has the necessary permissions to interact with your system's hardware and software components.

## Note
Future enhancements could explore integration with MIXXX DJ software, taking advantage of its API and open-source nature for a more adaptable and customizable DJing experience.
