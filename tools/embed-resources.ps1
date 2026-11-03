param(
  [Parameter(Mandatory = $true)][string]$Executable,
  [Parameter(Mandatory = $true)][string]$ResourceFile
)

$ErrorActionPreference = 'Stop'

$source = @'
using System;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

public static class ClicResourceEmbedder
{
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    private static extern IntPtr BeginUpdateResource(string fileName, bool deleteExistingResources);

    [DllImport("kernel32.dll", EntryPoint = "UpdateResourceW", SetLastError = true)]
    private static extern bool UpdateResource(IntPtr update, IntPtr type, IntPtr name, ushort language, IntPtr data, uint dataSize);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool EndUpdateResource(IntPtr update, bool discard);

    private sealed class ResourceId
    {
        public ushort? Ordinal;
        public string Name;
    }

    private static ushort ReadU16(byte[] bytes, int offset)
    {
        return BitConverter.ToUInt16(bytes, offset);
    }

    private static uint ReadU32(byte[] bytes, int offset)
    {
        return BitConverter.ToUInt32(bytes, offset);
    }

    private static ResourceId ReadId(byte[] bytes, ref int offset, int limit)
    {
        if (offset + 2 > limit) throw new InvalidDataException("Truncated resource identifier.");
        ushort value = ReadU16(bytes, offset);
        offset += 2;

        if (value == 0xffff)
        {
            if (offset + 2 > limit) throw new InvalidDataException("Truncated ordinal identifier.");
            ushort ordinal = ReadU16(bytes, offset);
            offset += 2;
            return new ResourceId { Ordinal = ordinal };
        }

        if (value == 0) return new ResourceId { Name = String.Empty };

        StringBuilder name = new StringBuilder();
        while (value != 0)
        {
            name.Append((char)value);
            if (offset + 2 > limit) throw new InvalidDataException("Unterminated resource identifier.");
            value = ReadU16(bytes, offset);
            offset += 2;
        }
        return new ResourceId { Name = name.ToString() };
    }

    private static IntPtr ToPointer(ResourceId id)
    {
        if (id.Ordinal.HasValue) return new IntPtr(id.Ordinal.Value);
        return Marshal.StringToHGlobalUni(id.Name ?? String.Empty);
    }

    public static int Embed(string executable, string resourceFile)
    {
        byte[] bytes = File.ReadAllBytes(resourceFile);
        IntPtr update = BeginUpdateResource(Path.GetFullPath(executable), false);
        if (update == IntPtr.Zero) throw new Win32Exception(Marshal.GetLastWin32Error());

        int count = 0;
        bool committed = false;
        try
        {
            int offset = 0;
            while (offset < bytes.Length)
            {
                if (offset + 8 > bytes.Length) throw new InvalidDataException("Truncated .res record header.");
                uint dataSize = ReadU32(bytes, offset);
                uint headerSize = ReadU32(bytes, offset + 4);
                if (headerSize < 8 || headerSize > Int32.MaxValue || offset + headerSize > bytes.Length)
                    throw new InvalidDataException("Invalid .res record header size.");

                int headerEnd = offset + (int)headerSize;
                int cursor = offset + 8;
                ResourceId type = ReadId(bytes, ref cursor, headerEnd);
                ResourceId name = ReadId(bytes, ref cursor, headerEnd);
                if (cursor + 12 > headerEnd) throw new InvalidDataException("Truncated .res metadata.");
                ushort language = ReadU16(bytes, cursor + 6);
                int dataOffset = headerEnd;
                if (dataSize > Int32.MaxValue || dataOffset + (long)dataSize > bytes.Length)
                    throw new InvalidDataException("Invalid .res record data size.");

                bool emptyHeader = dataSize == 0 && type.Ordinal == 0 && name.Ordinal == 0;
                if (!emptyHeader)
                {
                    IntPtr typePointer = ToPointer(type);
                    IntPtr namePointer = ToPointer(name);
                    IntPtr dataPointer = IntPtr.Zero;
                    try
                    {
                        if (dataSize != 0)
                        {
                            dataPointer = Marshal.AllocHGlobal((int)dataSize);
                            Marshal.Copy(bytes, dataOffset, dataPointer, (int)dataSize);
                        }
                        if (!UpdateResource(update, typePointer, namePointer, language, dataPointer, dataSize))
                            throw new Win32Exception(Marshal.GetLastWin32Error());
                    }
                    finally
                    {
                        if (dataPointer != IntPtr.Zero) Marshal.FreeHGlobal(dataPointer);
                        if (!type.Ordinal.HasValue) Marshal.FreeHGlobal(typePointer);
                        if (!name.Ordinal.HasValue) Marshal.FreeHGlobal(namePointer);
                    }
                    count++;
                }

                long next = (long)dataOffset + dataSize;
                offset = checked((int)((next + 3L) & ~3L));
            }

            if (!EndUpdateResource(update, false)) throw new Win32Exception(Marshal.GetLastWin32Error());
            committed = true;
            return count;
        }
        finally
        {
            if (!committed) EndUpdateResource(update, true);
        }
    }
}
'@

Add-Type -TypeDefinition $source
$count = [ClicResourceEmbedder]::Embed((Resolve-Path -LiteralPath $Executable).Path, (Resolve-Path -LiteralPath $ResourceFile).Path)
Write-Output "Embedded $count resources."
