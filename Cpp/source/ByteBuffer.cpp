/**
 *
 * Copyright (C) 2006-2009 Anton Gravestam.
 *
 * This file is part of OPS (Open Publish Subscribe).
 *
 * OPS (Open Publish Subscribe) is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * OPS (Open Publish Subscribe) is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OPS (Open Publish Subscribe).  If not, see <http://www.gnu.org/licenses/>.
 */
#include "OPSTypeDefs.h"
#include "ByteBuffer.h"

#undef ON_BIG_ENDIAN_MACHINE

namespace ops
{

    ByteBuffer::ByteBuffer(MemoryMap* mMap)
    {
        totalSize = 0;
        index = 0;
        memMap = mMap;
        currentSegment = 0;
        nextSegmentAt = memMap->getSegmentSize();
    }

    int ByteBuffer::getNrOfSegments()
    {
        return currentSegment + 1;
    }

    int ByteBuffer::getSegmentSize(int i)
    {
        if (i < currentSegment)
        {
            return memMap->getSegmentSize();
        }
        else
        {
            return index;
        }
    }

    char* ByteBuffer::getSegment(int i)
    {
        return memMap->getSegment(i);
    }

    void ByteBuffer::finish()
    {
        int oldIndex = index;
        int oldNextSegmentAt = nextSegmentAt;
        nextSegmentAt = 0;
        int nrSeg = getNrOfSegments();
        for (int i = 0; i < nrSeg; i++)
        {
            index = 6;
            currentSegment = i;
            nextSegmentAt += memMap->getSegmentSize();
            WriteInt(nrSeg);
        }
        index = oldIndex;
        nextSegmentAt = oldNextSegmentAt;
    }

    ByteBuffer::~ByteBuffer()
    {
    }

    void ByteBuffer::WriteChars(char* chars, int length)
    {
        int bytesLeftInSegment = memMap->getSegmentSize() - index;
        if (bytesLeftInSegment >= length)
        {
            memcpy((void*) (memMap->getSegment(currentSegment) + index), chars, length);
            index += length;
            totalSize += length;
        }
        else
        {
            memcpy((void*) (memMap->getSegment(currentSegment) + index), chars, bytesLeftInSegment);
            index += bytesLeftInSegment;
            totalSize += bytesLeftInSegment;
            nextSegmentAt += memMap->getSegmentSize();
            currentSegment++;
            writeNewSegment();
            WriteChars(chars + bytesLeftInSegment, length - bytesLeftInSegment);
        }
    }

    void ByteBuffer::writeNewSegment()
    {
        index = 0;
        writeProtocol();
        int tInt = 0;
        WriteInt(tInt); 
        WriteInt(currentSegment);
    }

    void ByteBuffer::readNewSegment()
    {
        index = 0;
        nextSegmentAt += memMap->getSegmentSize();
        bool ok = checkProtocol();
        int i1 = ReadInt();
        int i2 = ReadInt();
    }

    void ByteBuffer::ReadChars(char* chars, int length)
    {
        int bytesLeftInSegment = memMap->getSegmentSize() - index;
        if (bytesLeftInSegment >= length)
        {
            memcpy((void*) chars, memMap->getSegment(currentSegment) + index, length);
            index += length;
            totalSize += length;
        }
        else
        {
            memcpy((void*) chars, memMap->getSegment(currentSegment) + index, bytesLeftInSegment);
            index += bytesLeftInSegment;
            currentSegment++;
            totalSize += bytesLeftInSegment;
            readNewSegment();
            ReadChars(chars + bytesLeftInSegment, length - bytesLeftInSegment);
        }
    }

    void ByteBuffer::ByteSwap(unsigned char * b, int n)
    {
        register int i = 0;
        register int j = n - 1;
        while (i < j)
        {
            std::swap(b[i], b[j]);
            i++, j--;
        }
    }

    int ByteBuffer::GetSize()
    {
        return totalSize; 
    }

	// -----------------------------------------------------------------

    void ByteBuffer::WriteFloat(float f)
    {
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&f, 4);
#endif
        WriteChars(((char*)&f), 4);
    }

    void ByteBuffer::WriteInt(int i)
    {
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&i, 4);
#endif
        WriteChars(((char*)&i), 4);
    }

    void ByteBuffer::WriteShort(__int16 i)
    {
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&i, 2);
#endif
        WriteChars(((char*)&i), 2);
    }

    void ByteBuffer::WriteLong(__int64 l)
    {
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&l, 8);
#endif
        WriteChars(((char*)&l), 8);
    }

    void ByteBuffer::WriteDouble(double d)
    {
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&d, 8);
#endif
		WriteChars(((char*)&d), 8);
    }

    void ByteBuffer::WriteChar(char c)
    {
        WriteChars(&c, 1);
    }

    void ByteBuffer::WriteString(std::string& s)
    {
        int siz = (int) s.size();
        WriteInt(siz);
        WriteChars((char*) s.c_str(), s.size());
    }

	// -----------------------------------------------------------------

	//  void ByteBuffer::WriteOPSObject(OPSObject* o, OPSObjectHelper* oh)
    //  {
    //      int size = oh->getSize(o);
    ///*if(size > (Manager::MAX_SIZE - index))
    //{
    //	throw exceptions::CommException("In ByteBuffer::WriteOPSObject: Size exceeds MAX_SIZE limit.");
    //}*/
    //      WriteInt(size);
    //      //char* oBytes = oh->serialize(o, buffer + index);
    //      oh->serialize(o, buffer + index);
    //index += size;
    //      //delete oh;
    //  }

    /*void ByteBuffer::WriteOPSObjectFields(OPSObject* o)
    {
        
        WriteString(o->publisherName);
        WriteString(o->key);
        WriteInt(o->publicationID);
        WriteChar(o->publicationPriority);
        WriteString(o->typesString);
    }*/


    //Readers

    //  OPSObject* ByteBuffer::ReadOPSObject(OPSObjectHelper* oh)
    //  {
    //      int size = ReadInt();
    ///*if(size < 0)
    //{
    //	throw exceptions::CommException("In ByteBuffer::ReadOPSObject: Negative size read from data.");
    //}
    //if(size > Manager::MAX_SIZE)
    //{
    //	throw exceptions::CommException("In ByteBuffer::ReadOPSObject: Size of incomming data exceeds local MAX_SIZE limit.");
    //}*/
    //      //char* oBytes = new char[size];
    //      //ReadChars(oBytes, size);
    //      OPSObject* o = oh->deserialize(buffer + index);
    //index += size;
    //      //delete oh;
    //      return o;
    //  }

	// -----------------------------------------------------------------

    float ByteBuffer::ReadFloat()
    {
        float ret = 0;
        ReadChars((char*)&ret, 4);
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&ret, 4);
#endif
        return ret;
    }

    double ByteBuffer::ReadDouble()
    {
        double ret = 0;
        ReadChars((char*)&ret, 8);
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&ret, 8);
#endif
        return ret;
    }

    int ByteBuffer::ReadInt()
    {
        int ret = 0;
        ReadChars((char*)&ret, 4);
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&ret, 4);
#endif
        return ret;
    }

    __int16 ByteBuffer::ReadShort()
    {
        __int16 ret = 0;
        ReadChars((char*)&ret, 2);
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&ret, 2);
#endif
        return ret;
    }

    __int64 ByteBuffer::ReadLong()
    {
        __int64 ret = 0;
        ReadChars((char*)&ret, 8);
#ifdef ON_BIG_ENDIAN_MACHINE
        ByteSwap((unsigned char*)&ret, 8);
#endif
        return ret;
    }

    char ByteBuffer::ReadChar()
    {
        char ret = 0;
        ReadChars((char*)&ret, 1);
        return ret;
    }

    std::string ByteBuffer::ReadString()
    {
        int length = ReadInt();
        char* text = new char[length];
        ReadChars(text, length);
        std::string ret(text, length);
        delete[] text;
        return ret;
    }

	// -----------------------------------------------------------------

    /*void ByteBuffer::ReadOPSObjectFields(ops::OPSObject* o)
    {

            o->publisherName = ReadString();
            o->key = ReadString();
            o->publicationID = ReadInt();
            o->publicationPriority = ReadChar();
		    o->typesString = ReadString();
    }*/


    void ByteBuffer::ReadBooleans(std::vector<bool>& out)
    {
        int size = ReadInt();
        out.reserve(size);
        out.resize(size, false);
        for (int i = 0; i < size; i++)
        {
            out[i] = ReadChar() > 0;
        }
    }

    void ByteBuffer::WriteBooleans(std::vector<bool>& out)
    {
        int size = out.size();
        WriteInt(size);
        for (int i = 0; i < size; i++)
        {
            char ch = 0;
            out[i] ? ch = 1 : ch = 0;
            WriteChar(ch);
        }
    }

    void ByteBuffer::ReadBytes(std::vector<char>& out)
    {
        int length = ReadInt();
        out.reserve(length);
        out.resize(length, 0);
        ReadBytes(out, 0, length);
    }

    void ByteBuffer::ReadBytes(std::vector<char>& out, int offset, int length)
    {
        //int bytesLeftInSegment = nextSegmentAt - index;
        int bytesLeftInSegment = memMap->getSegmentSize() - index;
        std::vector<char>::iterator it = out.begin();
        it += offset;
        if (bytesLeftInSegment > length)
        {
            //memcpy((void*)(buffer + index), chars, length);
            //index += length;
            std::copy(memMap->getSegment(currentSegment) + index, memMap->getSegment(currentSegment) + index + length, it);
            index += length;
            totalSize += length;
        }
        else
        {
            //memcpy((void*)(buffer + index), chars, bytesLeftInSegment);
            //index += bytesLeftInSegment;

            //ReadBytes(out, offset, bytesLeftInSegment);

            std::copy(memMap->getSegment(currentSegment) + index, memMap->getSegment(currentSegment) + index + bytesLeftInSegment, it);
            index += bytesLeftInSegment;
            totalSize += bytesLeftInSegment;

            currentSegment++;
            readNewSegment();

            ReadBytes(out, offset + bytesLeftInSegment, length - bytesLeftInSegment);
            //WriteChars(chars + bytesLeftInSegment, length - bytesLeftInSegment);
        }
    }

    void ByteBuffer::WriteBytes(std::vector<char>& out)
    {
        int size = out.size();
        WriteInt(size);
        WriteBytes(out, 0, size);
    }

    void ByteBuffer::WriteBytes(std::vector<char>& out, int offset, int length)
    {
        int bytesLeftInSegment = memMap->getSegmentSize() - index;
        std::vector<char>::iterator it = out.begin();
        it += offset;
        if (bytesLeftInSegment > length)
        {
            std::copy(it, out.end(), memMap->getSegment(currentSegment) + index);
            index += length;
            totalSize += length;
        }
        else
        {
            std::copy(it, it + bytesLeftInSegment, memMap->getSegment(currentSegment) + index);
            index += bytesLeftInSegment;
            totalSize += bytesLeftInSegment;

            nextSegmentAt += memMap->getSegmentSize();
            currentSegment++;
            writeNewSegment();

            WriteBytes(out, offset + bytesLeftInSegment, length - bytesLeftInSegment);
        }
    }

    void ByteBuffer::ReadDoubles(std::vector<double>& out)
    {
        int size = ReadInt();
        out.reserve(size);
        out.resize(size, 0);
        if (size > 0)
        {
            ReadChars((char*) & out[0], size * 8);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 8);
            }
#endif
        }
    }

    void ByteBuffer::WriteDoubles(std::vector<double>& out)
    {
        int size = out.size();
        WriteInt(size);
        if (size > 0)
        {
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 8);
            }
#endif
            WriteChars((char*) & out[0], size * 8);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 8);
            }
#endif
        }
    }

    void ByteBuffer::ReadInts(std::vector<int>& out)
    {
        int size = ReadInt();
        out.reserve(size);
        out.resize(size, 0);
        if (size > 0)
        {
            ReadChars((char*) & out[0], size * 4);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 4);
            }
#endif
        }
    }

    void ByteBuffer::WriteInts(std::vector<int>& out)
    {
        int size = out.size();
        WriteInt(size);
        if (size > 0)
        {
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 4);
            }
#endif
            WriteChars((char*) & out[0], size * 4);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 4);
            }
#endif
        }
    }

    void ByteBuffer::ReadShorts(std::vector<__int16>& out)
    {
        int size = ReadInt();
        out.reserve(size);
        out.resize(size, 0);
        if (size > 0)
        {
            ReadChars((char*) & out[0], size * 2);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 2);
            }
#endif
        }
    }

    void ByteBuffer::WriteShorts(std::vector<__int16>& out)
    {
        int size = out.size();
        WriteInt(size);
        if (size > 0)
        {
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 2);
            }
#endif
            WriteChars((char*) & out[0], size * 2);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 2);
            }
#endif
        }
    }

    void ByteBuffer::ReadFloats(std::vector<float>& out)
    {
        int size = ReadInt();
        out.reserve(size);
        out.resize(size, 0.0);
        if (size > 0)
        {
            ReadChars((char*) & out[0], size * 4);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 4);
            }
#endif
        }
    }

    void ByteBuffer::WriteFloats(std::vector<float>& out)
    {
        int size = out.size();
        WriteInt(size);
        if (size > 0)
        {
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 4);
            }
#endif
            WriteChars((char*) & out[0], size * 4);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 4);
            }
#endif
        }
    }

    void ByteBuffer::ReadLongs(std::vector<__int64>& out)
    {
        int size = ReadInt();
        out.reserve(size);
        out.resize(size, 0);
        if (size > 0)
        {
            ReadChars((char*) & out[0], size * 8);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 8);
            }
#endif
        }
    }

    void ByteBuffer::WriteLongs(std::vector<__int64>& out)
    {
        int size = out.size();
        WriteInt(size);
        if (size > 0)
        {
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 8);
            }
#endif
            WriteChars((char*) & out[0], size * 8);
#ifdef ON_BIG_ENDIAN_MACHINE
            for(unsigned int i = 0; i < out.size() ; i++)
            {
                ByteSwap((unsigned char*) &out[i], 8);
            }
#endif
        }
    }

    void ByteBuffer::ReadStrings(std::vector<std::string>& out)
    {
        int size = ReadInt();
        out.reserve(size);
        out.resize(size, "");
        for (int i = 0; i < size; i++)
        {
            out[i] = ReadString();
        }
    }

    void ByteBuffer::WriteStrings(std::vector<std::string>& out)
    {
        int size = out.size();
        WriteInt(size);
        for (int i = 0; i < size; i++)
        {
            WriteString(out[i]);
        }
    }

    bool ByteBuffer::checkProtocol()
    {
        static char versionHigh = 0;
        static char versionLow = 5;
        static std::string protocolID("opsp");

        char inProtocolIDChars[5];
        ReadChars(&inProtocolIDChars[0], 4);
        inProtocolIDChars[4] = '\0';
        std::string inProtocolId(inProtocolIDChars);

        if (inProtocolId != protocolID)
        {
            return false;
        }

        char inVersionLow = ReadChar();
        char inVersionHigh = ReadChar();

        if ((inVersionHigh != versionHigh) || (inVersionLow != versionLow))
        {
            return false;
        }

        return true;
    }

    void ByteBuffer::writeProtocol()
    {
        static char versionHigh = 0;
        static char versionLow = 5;
        static std::string protocolID("opsp");

        WriteChars((char*) protocolID.c_str(), 4);

        WriteChar(versionLow);
        WriteChar(versionHigh);
    }

    void ByteBuffer::ResetIndex()
    {
        index = 0;
    }

}
