using System;
using System.Diagnostics;
using System.IO;
using System.Net.Sockets;
using System.Runtime.CompilerServices;
using System.Threading;

public class TcpConn
{
	public string String_0 { get; private set; }

	public int Int32_0 { get; private set; }

	public TcpConn(string string_1, int int_1)
	{
		this.String_0 = string_1;
		this.Int32_0 = int_1;
	}

	public void Connect()
	{
		this.tcpClient_0 = new TcpClient
		{
			NoDelay = true
		};
		IAsyncResult asyncResult = this.tcpClient_0.BeginConnect(this.String_0, this.Int32_0, null, null);
		WaitHandle asyncWaitHandle = asyncResult.AsyncWaitHandle;
		try
		{
			bool flag = !asyncResult.AsyncWaitHandle.WaitOne(TimeSpan.FromSeconds(5.0), false);
			if (flag)
			{
				this.tcpClient_0.Close();
				throw new IOException("Unable to Connect to the Wii U, connection timed-out", new TimeoutException());
			}
			this.tcpClient_0.EndConnect(asyncResult);
			asyncWaitHandle.Close();
		}
		catch
		{
			throw new IOException("Unable to connect to the Wii U, connection timed-out", new TimeoutException());
		}
		this.networkStream_0 = this.tcpClient_0.GetStream();
		Stream stream = this.networkStream_0;
		this.networkStream_0.WriteTimeout = 10000;
		stream.ReadTimeout = 10000;
	}

	public void Close()
	{
		try
		{
			bool flag = this.tcpClient_0 != null;
			if (flag)
			{
				this.tcpClient_0.Close();
				this.tcpClient_0.Dispose();
				this.networkStream_0.Close();
				this.networkStream_0.Dispose();
			}
		}
		catch (Exception)
		{
			throw new IOException("Unable to close the connection from the Wii U");
		}
		finally
		{
			this.tcpClient_0 = null;
			this.networkStream_0 = null;
		}
	}

	public void method_3(byte[] byte_0, uint uint_0, ref uint uint_1)
	{
		try
		{
			bool flag = this.networkStream_0 != null;
			if (flag)
			{
				int num = 0;
				uint_1 = 0u;
				while (uint_0 > 0u)
				{
					int num2 = this.networkStream_0.Read(byte_0, num, (int)uint_0);
					bool flag2 = num2 < 0;
					if (flag2)
					{
						break;
					}
					uint_1 += (uint)num2;
					num += num2;
					uint_0 -= (uint)num2;
				}
			}
		}
		catch (Exception innerException)
		{
			throw new IOException("Connection closed", innerException);
		}
	}

	public void method_5(byte[] byte_0, int int_1, ref uint uint_0)
	{
		try
		{
			bool flag = this.networkStream_0 == null;
			if (flag)
			{
				throw new IOException("The NetworkStream was null", new NullReferenceException());
			}
			this.networkStream_0.Write(byte_0, 0, int_1);
			bool flag2 = int_1 < 0;
			if (flag2)
			{
				uint_0 = 0u;
			}
			else
			{
				uint_0 = (uint)int_1;
			}
			this.networkStream_0.Flush();
		}
		catch (Exception innerException)
		{
			throw new IOException("Connection closed", innerException);
		}
	}

	private TcpClient tcpClient_0 = null;

	public NetworkStream networkStream_0 = null;
}