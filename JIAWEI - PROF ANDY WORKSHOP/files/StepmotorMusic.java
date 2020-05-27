package stepmotormusic;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortIOException;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.sound.midi.MetaMessage;
import javax.sound.midi.MidiEvent;
import javax.sound.midi.MidiMessage;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.Sequence;
import javax.sound.midi.ShortMessage;
import javax.sound.midi.SysexMessage;
import javax.sound.midi.Track;
import javax.swing.JOptionPane;
import javax.xml.bind.DatatypeConverter;

/**
 * @author Simon Spies
 */
public class StepmotorMusic implements Runnable {

    double factor = 757007;
    double exponent = -1.0011112619;
    char[] motors = {'X', 'Y', 'Z', 'A'};
    
    Double[] midiNoteNumFreqs = {8.18,8.66,9.18,9.72,10.30,10.91,11.56,12.25,12.98,13.75,14.57,15.43,16.35,17.32,18.35,19.45,20.60,21.83,23.12,24.50,25.96,27.50,29.14,30.87,32.70,34.65,36.71,38.89,41.20,43.65,46.25,49.00,51.91,55.00,58.27,61.74,65.41,69.30,73.42,77.78,82.41,87.31,92.50,98.00,103.83,110.00,116.54,123.47,130.81,138.59,146.83,155.56,164.81,174.61,185.00,196.00,207.65,220.00,233.08,246.94,261.63,277.18,293.66,311.13,329.63,349.23,369.99,392.00,415.30,440.00,466.16,493.88,523.25,554.37,587.33,622.25,659.26,698.46,739.99,783.99,830.61,880.00,932.33,987.77,1046.50,1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760.00,1864.66,1975.53,2093.00,2217.46,2349.32,2489.02,2637.02,2793.83,2959.96,3135.96,3322.44,3520.00,3729.31,3951.07,4186.01,4434.92,4698.64,4978.03,5274.04,5587.65,5919.91,6271.93,6644.88,7040.00,7458.62,7902.13,8372.02,8869.84,9397.27,9956.06,10548.08,11175.30,11839.82,12543.85};
    String path, serialPort;
    Main root;
    
    public StepmotorMusic(Main root, String path, String serialPort) {
        this.path = path;
        this.serialPort = serialPort;
        this.root = root;
    }
    
    public static List<String> getSerialPorts() throws Exception {
        List<String> portList = new ArrayList<>();
        
        SerialPort[] ports = SerialPort.getCommPorts();
        for (SerialPort port : ports) 
            portList.add(port.getSystemPortName());
        
        return portList;
    }
    
    @Override
    public void run() {
        try {
            SerialPort arduino = SerialPort.getCommPort(serialPort); // Create a serial connection
        
            try {
                arduino.openPort(); // Open the connection
                
                if (!arduino.isOpen())
                    throw new Exception("Unable to open Port!");
                
                arduino.setComPortParameters(9600, 8, SerialPort.ONE_STOP_BIT, SerialPort.NO_PARITY); // Initialize the connection
                arduino.setFlowControl(SerialPort.FLOW_CONTROL_RTS_ENABLED);
                arduino.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0, 0);
                
                arduino.getOutputStream().write(new byte[] {(byte) 'E', 0, 1}); // Start the stepdrivers
                arduino.getOutputStream().flush();
                
                File inFile = new File(path);
        
                if (!inFile.exists() || inFile.isDirectory() || !inFile.canRead())
                    throw new Exception("Opening input File failed!");
        
                Sequence sequence = MidiSystem.getSequence(inFile);
        
                long microsecondsPerTick, microsecondsPerQuarterNote = 500000L; // Default 120 BPM
                if (sequence.getDivisionType() == Sequence.PPQ)
                    microsecondsPerTick = microsecondsPerQuarterNote / sequence.getResolution();
                else
                    microsecondsPerTick = Math.round(1000000d / sequence.getDivisionType() / sequence.getResolution());
        
                List<Long> timestamps = new ArrayList<>();
                Map<Long, List<byte[]>> commands = new HashMap<>();
                int maxChannelNum = 0;

TrackLoop:
                for (Track track : sequence.getTracks()) {
                    long curTime = 0, lastTicks = 0;
                    int[] curNote = {-1, -1, -1, -1};
                    int[] playing = {0, 0, 0, 0};
            
                    for (int i = 0; i < track.size(); i++) {
                        MidiEvent event = track.get(i);
                        curTime += (event.getTick() - lastTicks) * microsecondsPerTick;
                        lastTicks = event.getTick();
                
                        MidiMessage message = event.getMessage();
                        if (message instanceof MetaMessage) {
                            MetaMessage metaMessage = (MetaMessage) message;
                            byte[] metaMessageData = metaMessage.getData();
                    
                            switch (metaMessage.getType()) {
                                case 0x00:
                                    if (metaMessageData.length != 2 || curTime != 0L)
                                        throw new Exception("Invalid sequence number event!");

                                    int sequenceNumber = (Byte.toUnsignedInt(metaMessageData[0]) << 8) | Byte.toUnsignedInt(metaMessageData[1]);
                                    System.out.println("INFO: Current sequence Number is: " + String.valueOf(sequenceNumber));
                                    break;
                            
                                case 0x01:
                                    String textMessage = new String(metaMessageData);
                                    System.out.println("INFO: Text Message: " + textMessage);
                                    break;
                            
                                case 0x02:
                                    String copyright = new String(metaMessageData);
                                    System.out.println("INFO: Copyright: " + copyright);
                                    break;
                            
                                case 0x03:
                                    String name = new String(metaMessageData);
                                    System.out.println("INFO: Sequence/Track Name: " + name);
                                    break;
                            
                                case 0x04:
                                    String instrument = new String(metaMessageData);
                                    System.out.println("INFO: Instrument Name: " + instrument);
                                    break;
                            
                                case 0x05:
                                    String lyrics = new String(metaMessageData);
                                    System.out.println("INFO: Lyrics at " + String.format("%02d:%02d:%02d", curTime/3600000000L, (curTime/60000000) % 60, (curTime/1000000) % 60) + " : " + lyrics);
                                    break;
                            
                                case 0x06:
                                    String marker = new String(metaMessageData);
                                    System.out.println("INFO: Marker at " + String.format("%02d:%02d:%02d", curTime/3600000000L, (curTime/60000000) % 60, (curTime/1000000) % 60) + " : " + marker);
                                    break;
                            
                                case 0x07:
                                    String specialEvent = new String(metaMessageData);
                                    System.out.println("INFO: Special event at " + String.format("%02d:%02d:%02d", curTime/3600000000L, (curTime/60000000) % 60, (curTime/1000000) % 60) + " : " + event);
                                    break;
                            
                                case 0x20:
                                    if (metaMessageData.length != 1)
                                        throw new Exception("Invalid channel change event!");
                        
                                    System.out.println("INFO: Switch to channel " + String.valueOf(Byte.toUnsignedInt(metaMessageData[0])));
                                    break;
                            
                                case 0x2F:
                                    if (metaMessageData.length != 0)
                                        throw new Exception("Invalid end of track event!");
                        
                                    if (i != track.size() - 1)
                                        System.out.println("WARENING: Found end of track event before end of track!");
                            
                                    continue TrackLoop;
                            
                                case 0x51:
                                    if (metaMessageData.length != 3)
                                        throw new Exception("Invalid speed change event!");
                            
                                    if (sequence.getDivisionType() == Sequence.PPQ) {                    
                                        microsecondsPerQuarterNote = (Byte.toUnsignedInt(metaMessageData[0]) << 16) | (Byte.toUnsignedInt(metaMessageData[1]) << 8) | Byte.toUnsignedInt(metaMessageData[2]);
                                        microsecondsPerTick = microsecondsPerQuarterNote / sequence.getResolution();
                                    } else
                                        System.out.println("WARNING: Found speed change event but Sequence is of SMPTE type!");
                                    break;
                            
                                case 0x54:
                                    if (metaMessageData.length != 5 || curTime != 0L)
                                        throw new Exception("Invalid SMPTE offset event!");
                            
                                    float[] SMPTETypes = {Sequence.SMPTE_24, Sequence.SMPTE_25, Sequence.SMPTE_30DROP, Sequence.SMPTE_30};
                                    curTime += (Byte.toUnsignedInt(metaMessageData[0]) & 0x1F) * 3600000000L;
                                    curTime += Byte.toUnsignedInt(metaMessageData[1]) * 60000000L;
                                    curTime += Byte.toUnsignedInt(metaMessageData[2]) * 1000000L;
                                    curTime += Byte.toUnsignedInt(metaMessageData[3]) * 1000000L / SMPTETypes[(Byte.toUnsignedInt(metaMessageData[0]) & 0x60) >>> 5];
                                    curTime += Byte.toUnsignedInt(metaMessageData[4]) * 10000L / SMPTETypes[(Byte.toUnsignedInt(metaMessageData[0]) & 0x60) >>> 5];
                                    break;
                            
                                case 0x58:
                                    if (metaMessageData.length != 4)
                                        throw new Exception("Invalid time signature event!");
                            
                                    System.out.println("INFO: Time signature: " + String.valueOf(Byte.toUnsignedInt(metaMessageData[0])) + "/" + String.valueOf(Math.round(Math.pow(2, Byte.toUnsignedInt(metaMessageData[1])))) + " with " + String.valueOf(Byte.toUnsignedInt(metaMessageData[2])) + " midi clocks per metronome tick and " + String.valueOf(Byte.toUnsignedInt(metaMessageData[3])) + "/32 notes per 24 midi clocks!");
                                    break;
                            
                                case 0x59:
                                    if (metaMessageData.length != 2)
                                        throw new Exception("Invalid key signature event!");
                        
                                    // Not implemented!
                                    break;
                            
                                case 0x7F:
                                    // Not implemented!
                                    break;
                        
                                default:
                                    System.out.println("Found invalid meta event with key " + Integer.toHexString(metaMessage.getType()));
                            }
                        }
                        else if (message instanceof SysexMessage) {
                            SysexMessage sysexMessage = (SysexMessage) message;
                            System.out.println("INFO: MIDI Message: " + ((sysexMessage.getStatus() == 0xF0) ? "F0 " : "") + new String(sysexMessage.getData()));
                        }
                        else if (message instanceof ShortMessage) {
                            ShortMessage shortMessage = (ShortMessage) message;
                            int channel = shortMessage.getChannel();
                            maxChannelNum = Math.max(channel, maxChannelNum);
                    
                            switch (shortMessage.getCommand()) {
                                case ShortMessage.NOTE_OFF:
                                    if (channel < 4) {
                                        if (!timestamps.contains(curTime)) {
                                            timestamps.add(curTime);
                                            commands.put(curTime, new ArrayList<>());
                                        }
                                        
                                        playing[channel]--;
                                        if (playing[channel] == 0)
                                            curNote[channel] = -1;
                                        
                                        byte[] command = {(byte) channel, 0, 0};
                                        commands.get(curTime).add(command);
                                    }
                                    break;
                            
                                case ShortMessage.NOTE_ON:
                                    if (channel < 4) {
                                        if (!timestamps.contains(curTime)) {
                                            timestamps.add(curTime);
                                            commands.put(curTime, new ArrayList<>());
                                        }
                                
                                        playing[channel]++;
                                        curNote[channel] = shortMessage.getData1();
                                        
                                        short speed = (short) Math.round(Math.pow(midiNoteNumFreqs[shortMessage.getData1()] / factor, 1./exponent));
                                        byte[] command = {(byte) channel, (byte) (speed >>> 8), (byte) (speed & 0x00FF)};
                                        commands.get(curTime).add(command);
                                    }
                                    break;
                            
                                case ShortMessage.PITCH_BEND:
                                    if (curNote[channel] == -1)
                                        break;
                                    
                                    if (!timestamps.contains(curTime)) {
                                        timestamps.add(curTime);
                                        commands.put(curTime, new ArrayList<>());
                                    }
                                    
                                    short pitchBendValue = (short) (((shortMessage.getData2() & 0x7F) << 7) | (shortMessage.getData1() & 0x7F));
                                    short curSpeed = (short) Math.round(Math.pow(midiNoteNumFreqs[curNote[channel]]                    / factor, 1./exponent));
                                    short minSpeed = (short) Math.round(Math.pow(midiNoteNumFreqs[Math.max(curNote[channel] - 2, 0)]   / factor, 1./exponent));
                                    short maxSpeed = (short) Math.round(Math.pow(midiNoteNumFreqs[Math.min(curNote[channel] + 2, 127)] / factor, 1./exponent));
                                    short newSpeed = (pitchBendValue >= 0x2000) ?
                                                     (short) (curSpeed - ((curSpeed - maxSpeed) * (pitchBendValue - 0x2000) / 0x1FFF)) :
                                                     (short) (minSpeed - ((minSpeed - curSpeed) * pitchBendValue / 0x1FFF));
                                    
                                    byte[] command = new byte[] {(byte) channel, 0, 0};
                                    commands.get(curTime).add(command);
                                    command = new byte[] {(byte) channel, (byte) (newSpeed >>> 8), (byte) (newSpeed & 0x00FF)};
                                    commands.get(curTime).add(command);
                                    
                                    break;
                                    
                                default:
                                    // All others are not implemented!
                            }
                        }
                        else
                            System.out.println("WARNING: Message of invalid type found: " + DatatypeConverter.printHexBinary(message.getMessage()));
                    }
                }

                System.out.println("Found " + String.valueOf(maxChannelNum + 1) + " Channels!");
        
                Collections.sort(timestamps);
                
                int[] playing = {0, 0, 0, 0};
                
                for (Long timestamp : timestamps) {
                    List<byte[]> commandList = commands.get(timestamp);
                    boolean[] containsNoteOn  = {false, false, false, false};
                    //boolean[] containsNoteOff = {false, false, false, false};
                    
                    for (byte[] command : commandList) {
                        containsNoteOn [command[0]] |= ((command[1] != 0) || (command[2] != 0));
                        //containsNoteOff[command[0]] |= ((command[1] == 0) && (command[2] == 0));
                    }
                    
                    for (int i = 0; i < commandList.size(); i++) {
                        byte[] command = commandList.get(i);
                        
                        if ((command[1] != 0) || (command[2] != 0))
                            playing[command[0]]++;
                        else {
                            playing[command[0]]--;
                            
                            if (containsNoteOn[command[0]] || playing[command[0]] > 0)
                                commandList.remove(i--);
                        }
                    }
                }
                
                long lastTime = 0;
            
                for (Long timestamp : timestamps) {
                    Thread.sleep((timestamp - lastTime) / 1000);
                    lastTime = timestamp;
            
                    for (byte[] command : commands.get(timestamp)) {
                        command[0] = (byte) motors[command[0]];
                        arduino.getOutputStream().write(command);
                        arduino.getOutputStream().flush();
                    }
                }
                
                arduino.getOutputStream().write(new byte[] {(byte) 'E', 0, 0}); // Stop the stepdrivers
                arduino.getOutputStream().flush();
                arduino.closePort();
            } catch (InterruptedException e) {
                if (arduino.isOpen()) {
                    arduino.getOutputStream().write(new byte[] {(byte) 'X', 0, 0, (byte) 'Y', 0, 0, (byte) 'Z', 0, 0, (byte) 'A', 0, 0, (byte) 'E', 0, 0}); // Stop the motors and disable the stepdrivers
                    arduino.getOutputStream().flush();
                    arduino.closePort();
                    root.playThreadInterrupted();
                }
            }
        } catch (SerialPortIOException e) {
            JOptionPane.showMessageDialog(null, "There is an error on writing string to port: " + e.getMessage(), "Serial connection error!", JOptionPane.ERROR_MESSAGE);
            e.printStackTrace(System.err);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, e.getMessage(), "Serial connection error!", JOptionPane.ERROR_MESSAGE);
            e.printStackTrace(System.err);
        }

        Thread.currentThread().interrupt();
        root.playThreadInterrupted();
    }
}
