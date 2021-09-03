using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

public class Gecko
{
	public Gecko(string string_0)
	{
		tcpConn = new TcpConn(string_0, Gecko.port);
	}

	protected Gecko.GEnum0 method_0(byte[] byte_0, uint uint_3)
	{
		uint num = 0u;
		try
		{
			tcpConn.method_3(byte_0, uint_3, ref num);
		}
		catch (IOException)
		{
			tcpConn.Close();
			return Gecko.GEnum0.CMD_FatalError;
		}
		bool flag = num != uint_3;
		Gecko.GEnum0 result;
		if (flag)
		{
			result = Gecko.GEnum0.CMD_ResultError;
		}
		else
		{
			result = Gecko.GEnum0.CMD_OK;
		}
		return result;
	}

	protected Gecko.GEnum0 method_1(byte[] byte_0, int int_1)
	{
		uint num = 0u;
		try
		{
			tcpConn.method_5(byte_0, int_1, ref num);
		}
		catch (IOException)
		{
			tcpConn.Close();
			return Gecko.GEnum0.CMD_FatalError;
		}
		bool flag = (ulong)num != (ulong)((long)int_1);
		Gecko.GEnum0 result;
		if (flag)
		{
			result = Gecko.GEnum0.CMD_ResultError;
		}
		else
		{
			result = Gecko.GEnum0.CMD_OK;
		}
		return result;
	}

	public Gecko.GEnum0 SendCommand(byte byte_0)
	{
		return method_1(BitConverter.GetBytes((short)byte_0), 1);
	}

	private void SendCommand(Class10.Enum0 enum0_0)
	{
		uint num = 0u;
		tcpConn.method_5(new byte[]
		{
			(byte)enum0_0
		}, 1, ref num);
	}

	public byte[] method_4(uint uint_3, uint uint_4)
	{
		try
		{
			method_5(uint_3, uint_4);
			uint num = 0u;
			byte[] array = new byte[1];
			tcpConn.method_3(array, 1u, ref num);
			MemoryStream memoryStream = new MemoryStream();
			bool flag = array[0] != 176;
			if (flag)
			{
				uint num3;
				for (uint num2 = uint_4; num2 > 0u; num2 -= num3)
				{
					num3 = num2;
					bool flag2 = num3 > uint_2;
					if (flag2)
					{
						num3 = uint_2;
					}
					byte[] array2 = new byte[num3];
					num = 0u;
					tcpConn.method_3(array2, num3, ref num);
					memoryStream.Write(array2, 0, (int)num3);
				}
				return memoryStream.ToArray();
			}
			return memoryStream.ToArray();
		}
		catch (Exception)
		{
		}
		return null;
	}

	private void method_5(uint uint_3, uint uint_4)
	{
		try
		{
			SendCommand(Class10.Enum0.COMMAND_READ_MEMORY);
			uint num = 0u;
			byte[] bytes = BitConverter.GetBytes(GClass1.meth1(uint_3));
			byte[] bytes2 = BitConverter.GetBytes(GClass1.meth1(uint_3 + uint_4));
			tcpConn.method_5(bytes, 4, ref num);
			tcpConn.method_5(bytes2, 4, ref num);
		}
		catch (Exception)
		{
		}
	}

	public uint method_6(uint uint_3, byte[] byte_0)
	{
		int int_ = byte_0.Length;
		uint result = uint_3 + (uint)byte_0.Length;
		uint num = 0u;
		tcpConn.method_5(byte_0, int_, ref num);
		return result;
	}

	public byte[] string2bytes(string string_0)
	{
		byte[] array = new byte[string_0.Length / 2];
		int num = 0;
		for (int i = 0; i < string_0.Length; i += 2)
		{
			string value = string_0.Substring(i, 2);
			array[num] = Convert.ToByte(value, 16);
			num++;
		}
		return array;
	}

	private void UploadMemory(uint uint_3, IEnumerable<byte[]> ienumerable_0)
	{
		IEnumerable<byte[]> source = (ienumerable_0 as IList<byte[]>) ?? ienumerable_0.ToList<byte[]>();
		uint num = (uint)source.Sum(new Func<byte[], int>(Gecko.GetByteAmount));
		try
		{
			SendCommand(Class10.Enum0.COMMAND_UPLOAD_MEMORY);
			uint num2 = 0u;
			byte[] bytes = BitConverter.GetBytes(GClass1.meth1(uint_3));
			byte[] bytes2 = BitConverter.GetBytes(GClass1.meth1(uint_3 + num));
			tcpConn.method_5(bytes, 4, ref num2);
			tcpConn.method_5(bytes2, 4, ref num2);
			source.Aggregate(uint_3, new Func<uint, byte[], uint>(method_6));
		}
		catch (Exception ex)
		{
			Console.Write(ex.Message);
		}
	}

	private static IEnumerable<byte[]> smethod_0(byte[] byte_0, uint uint_3)
	{
		List<byte[]> list = new List<byte[]>();
		uint num = 0u;
		while ((ulong)num < (ulong)((long)byte_0.Length))
		{
			long long_ = Math.Min((long)byte_0.Length, (long)((ulong)(num + uint_3)));
			list.Add(Gecko.smethod_1(byte_0, (long)((ulong)num), long_));
			num += uint_3;
		}
		return list;
	}

	private static byte[] smethod_1(byte[] byte_0, long long_0, long long_1)
	{
		long num = long_1 - long_0;
		byte[] array = new byte[num];
		Array.Copy(byte_0, long_0, array, 0L, num);
		return array;
	}
	public void WriteBytes(uint uint_3, byte[] byte_0)
	{
		IEnumerable<byte[]> ienumerable_ = Gecko.smethod_0(byte_0, uint_2);
		UploadMemory(uint_3, ienumerable_);
	}
	public uint CallFunction(uint uint_3, params uint[] uint_4)
	{
		return (uint)(CallFunction64(uint_3, uint_4) >> 32);
	}
	public ulong CallFunction64(uint uint_3, params uint[] uint_4)
	{
		byte[] array = new byte[36];
		uint_3 = GClass1.meth1(uint_3);
		BitConverter.GetBytes(uint_3).CopyTo(array, 0);
		for (int i = 0; i < 8; i++)
		{
			bool flag = i < uint_4.Length;
			if (flag)
			{
				BitConverter.GetBytes(GClass1.meth1(uint_4[i])).CopyTo(array, 4 + i * 4);
			}
			else
			{
				BitConverter.GetBytes(4274704570u).CopyTo(array, 4 + i * 4);
			}
		}
		if (SendCommand(112) != Gecko.GEnum0.CMD_OK)
		{
			throw new GeckoException(GeckoException.GEnum1.FTDICommandSendError);
		}
		if (method_1(array, array.Length) != Gecko.GEnum0.CMD_OK)
		{
			throw new GeckoException(GeckoException.GEnum1.FTDICommandSendError);
		}
		if (method_0(array, 8u) != Gecko.GEnum0.CMD_OK)
		{
			throw new GeckoException(GeckoException.GEnum1.FTDICommandSendError);
		}
		return GClass1.meth2(BitConverter.ToUInt64(array, 0));
	}
	public void PokeUInt(uint uint_3, uint uint_4)
	{
		byte[] bytes = BitConverter.GetBytes(uint_4);
		try
		{
			Array.Reverse(bytes);
		}
		catch (Exception ex)
		{
			Console.Write(ex.Message);
		}
		WriteBytes(uint_3, bytes);
	}
	public void PokeInt(uint uint_3, int int_1)
	{
		byte[] bytes = BitConverter.GetBytes(int_1);
		try
		{
			Array.Reverse(bytes);
		}
		catch (Exception ex)
		{
			Console.Write(ex.Message);
		}
		WriteBytes(uint_3, bytes);
	}
	public uint PeekUInt(uint uint_3)
	{
		byte[] array = method_4(uint_3, 4u);
		uint result;
		try
		{
			Array.Reverse(array);
			result = ((!array.Any<byte>()) ? 0u : BitConverter.ToUInt32(array, 0));
		}
		catch (Exception)
		{
			return 0u;
		}
		return result;
	}
	public static int GetByteAmount(byte[] array)
	{
		return array.Length;
	}

	public TcpConn tcpConn;

	private static int port = 7331;
	private uint uint_2 = 1024u;

	public enum GEnum0
	{
		CMD_ResultError,
		CMD_FatalError,
		CMD_OK
	}
}