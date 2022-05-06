using System;

public static class GClass1
{
	public static ushort meth0(ushort ushort_0)
	{
		bool isLittleEndian = BitConverter.IsLittleEndian;
		ushort result;
		if (isLittleEndian)
		{
			result = (ushort)((65280 & ushort_0) >> 8 | (int)(255 & ushort_0) << 8);
		}
		else
		{
			result = ushort_0;
		}
		return result;
	}

	public static uint meth1(uint uint_0)
	{
		bool isLittleEndian = BitConverter.IsLittleEndian;
		uint result;
		if (isLittleEndian)
		{
			result = ((4278190080u & uint_0) >> 24 | (16711680u & uint_0) >> 8 | (65280u & uint_0) << 8 | (255u & uint_0) << 24);
		}
		else
		{
			result = uint_0;
		}
		return result;
	}

	public static ulong meth2(ulong ulong_0)
	{
		bool flag = !BitConverter.IsLittleEndian;
		ulong result;
		if (flag)
		{
			result = ulong_0;
		}
		else
		{
			result = ((18374686479671623680UL & ulong_0) >> 56 | (71776119061217280UL & ulong_0) >> 40 | (280375465082880UL & ulong_0) >> 24 | ((1095216660480UL & ulong_0) >> 8) | ((0xFFFFFFFFFF000000 & ulong_0) << 8) | (16711680UL & ulong_0) << 24 | (65280UL & ulong_0) << 40 | (255UL & ulong_0) << 56);
		}
		return result;
	}
}
