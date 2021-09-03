using System;

public class GeckoException : Exception
{
	public GeckoException(GeckoException.GEnum1 genum1_1)
	{
		this.genum1_0 = genum1_1;
	}

	private GeckoException.GEnum1 genum1_0;

	public enum GEnum1
	{
		FTDIQueryError,
		noFTDIDevicesFound,
		noTCPGeckoFound,
		FTDIResetError,
		FTDIPurgeRxError,
		FTDIPurgeTxError,
		FTDITimeoutSetError,
		FTDITransferSetError,
		FTDICommandSendError,
		FTDIReadDataError,
		FTDIInvalidReply,
		TooManyRetries,
		REGStreamSizeInvalid,
		CheatStreamSizeInvalid
	}
}