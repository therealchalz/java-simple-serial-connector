package jssc;

import java.io.IOException;
import java.io.InputStream;

/**
 * Class that wraps a {@link SerialPort} to provide
 * {@link InputStream} functionality.  This stream
 * also provides support for performing blocking reads 
 * with timeouts.
 * <br>
 * It is instantiated by passing the constructor a {@link SerialPort}
 * instance.  Do not create multiple streams for the 
 * same serial port unless you implement your own
 * synchronization.
 * @author Charles Hache <chalz@member.fsf.org>
 *
 */
public class SerialInputStream extends InputStream {
	
	private SerialPort serialPort;
	private int defaultTimeout = 0;

	/** Instantiates a SerialInputStream for the given {@link SerialPort}
	 * Do not create multiple streams for the same serial port
	 * unless you implement your own synchronization.
	 * @param sp The serial port to stream.
	 */
	public SerialInputStream(SerialPort sp) {
		serialPort = sp;
	}
	
	/** Set the default timeout (ms) of this SerialInputStream.
	 * This affects subsequent calls to {@link #read()}, {@link #blockingRead(int[])}, 
	 * and {@link #blockingRead(int[], int, int)}
	 * The default timeout can be 'unset' by setting it to -1, which can cause indefinite blocks.
	 * @param time The timeout in milliseconds.
	 */
	public void setTimeout(int time) {
		defaultTimeout = time;
	}

	/** Reads the next byte from the port.
	 * If the timeout of this stream has been set, then this method
	 * blocks until data is available or until the timeout has elapsed.
	 * If the timeout is not set or has been set to -1, then this method
	 * blocks indefinitely.  If the timeout is set to 0, then this method 
	 * attempts to return immediately.  If there is no data available, then
	 * throws an exception.
	 */
	@Override
	public int read() throws IOException {
		return read(defaultTimeout);
	}
	
	/** The same contract as {@link #read()}, except overrides
	 * this stream's default timeout with the given
	 * timeout in milliseconds.
	 * @param timeout The timeout in milliseconds. -1 to block forever. 0 to 
	 * attempt to return immediately.  If there is not data available, then
	 * throws an exception.
	 * @return The read byte.
	 * @throws IOException On serial port error or timeout
	 */
	public int read(int timeout) throws IOException {
		byte[] data = serialPort.readBytes(1, timeout);
		if (data.length==0) {
			throw new SerialPortTimeoutException(serialPort.getPortName(), "read(int timeout)", timeout);
		}
		return data[0];
	}
	
	/** Non-blocking read of up to buf.length bytes from the stream.
	 * This call behaves as read(buf, 0, buf.length) would.
	 * @param buf The buffer to fill.
	 * @return The number of bytes read, which can be 0.
	 * @throws IOException on error.
	 */
	@Override
	public int read(byte[] buf) throws IOException {
		return read(buf, 0, buf.length);
	}
	
	/** Non-blocking read of up to length bytes from the stream.
	 * This method returns what is immediately available in the input
	 * buffer.
	 * @param buf The buffer to fill.
	 * @param offset The offset into the buffer to start copying data.
	 * @param length The maximum number of bytes to read.
	 * @return The actual number of bytes read, which can be 0.
	 * @throws IOException on error.
	 */
	@Override
	public int read(byte[] buf, int offset, int length) throws IOException {
		
		if (buf.length < offset + length)
			length = buf.length - offset;
		
		byte[] readBuf = serialPort.readBytesWithTimeout(length, 0, false);
		System.arraycopy(readBuf, 0, buf, offset, readBuf.length);
		return readBuf.length;
	}
	
	/** Blocks until buf.length bytes are read, an error occurs, or the default timeout is hit (if specified).
	 * This behaves as blockingRead(buf, 0, buf.length) would.
	 * @param buf The buffer to fill with data.
	 * @return The number of bytes read.
	 * @throws IOException On error or timeout.
	 */
	public int blockingRead(byte[] buf) throws IOException {
		return blockingRead(buf, 0, buf.length, defaultTimeout);
	}
	
	/** The same contract as {@link #blockingRead(byte[])} except
	 * overrides this stream's default timeout with the given one.
	 * @param buf The buffer to fill.
	 * @param timeout The timeout in milliseconds.
	 * @return The number of bytes read.
	 * @throws IOException On error or timeout.
	 */
	public int blockingRead(byte[] buf, int timeout) throws IOException {
		return blockingRead(buf, 0, buf.length, timeout);
	}
	/** Blocks until length bytes are read, an error occurs, or the default timeout is hit (if specified).
	 * Saves the data into the given buffer at the specified offset.
	 * If the stream's timeout is not set, behaves as {@link #read(byte[], int, int)} would.
	 * @param buf The buffer to fill.
	 * @param offset The offset in buffer to save the data.
	 * @param length The number of bytes to read.
	 * @return the number of bytes read.
	 * @throws IOException on error or timeout.
	 */
	public int blockingRead(byte[] buf, int offset, int length) throws IOException {
		return blockingRead(buf, offset, length, defaultTimeout);
	}
	
	/** The same contract as {@link #blockingRead(byte[], int, int)} except
	 * overrides this stream's default timeout with the given one.
	 * @param buf The buffer to fill.
	 * @param offset Offset in the buffer to start saving data.
	 * @param length The number of bytes to read.
	 * @param timeout The timeout in milliseconds.
	 * @return The number of bytes read.
	 * @throws IOException On error or timeout.
	 */
	public int blockingRead(byte[] buf, int offset, int length, int timeout) throws IOException {
		if (buf.length < offset + length)
			throw new IOException("Not enough buffer space for serial data");
		
		byte[] readBuf = serialPort.readBytesWithTimeout(length, timeout, true);
		System.arraycopy(readBuf, 0, buf, offset, readBuf.length);
		return readBuf.length;
	}
	
	@Override
	public int available() throws IOException {
		int ret;
		try {
			ret = serialPort.getInputBufferBytesCount();
			if (ret >= 0) 
				return ret;
			throw new IOException("Error checking available bytes from the serial port.");
		} catch (Exception e) {
			throw new IOException("Error checking available bytes from the serial port.");
		} 
	}

}
