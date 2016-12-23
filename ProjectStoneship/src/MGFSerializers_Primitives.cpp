
/**
 * Serializers for primitive types (integrals, floats, etc.)
 *  I have a strong feeling "primitive" is a thing only Java programmers say...
 */

#include "MGFSerializers_Primitives.h"

#include "MGFDataReader.h"
#include "MGFDataWriter.h"
#include "Logger.h"

#include <type_traits>

namespace Stoneship
{
    
    //---- begin writers ----
    
    
    /* specialization for all integral types (hopefully!)
          NOPE, doesn't work!
    template <typename T>
	typename std::enable_if<std::is_integral<T>::value, MGFDataWriter&>::type MGFDataWriter::operator << <T>(const T &v)
	{
		_stupidlyWriteIntegral<T>(v);

		return *this;
	}*/
	
	// Aww, c'mon. Do we really have to do this for _every_ integral type?
	// I wish partial specialization would be less of a hassle...
	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint64_t>(const uint64_t &l)
	{
		_stupidlyWriteIntegral<uint64_t>(l);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint32_t>(const uint32_t &i)
	{
		_stupidlyWriteIntegral<uint32_t>(i);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint16_t>(const uint16_t &s)
	{
		_stupidlyWriteIntegral<uint16_t>(s);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint8_t>(const uint8_t &b)
	{
		_writeNext(b);

		return *this;
	}
	
	template <>
	MGFDataWriter &MGFDataWriter::operator << <int64_t>(const int64_t &l)
	{
		_stupidlyWriteIntegral<int64_t>(l);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <int32_t>(const int32_t &i)
	{
		_stupidlyWriteIntegral<int32_t>(i);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <int16_t>(const int16_t &s)
	{
		_stupidlyWriteIntegral<int16_t>(s);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <int8_t>(const int8_t &b)
	{
		_writeNext(b);

		return *this;
	}
	
	
	//FIXME: Portability issue. This could produce invalid results if target platform uses different floating point format than the advised IEEE 754
	template <>
    MGFDataWriter &MGFDataWriter::operator << <float>(const float &f)
    {
	    union
		{
			float v;
			char data[sizeof(v)]; // evil!
		};

		v = f;
		
		for(uint8_t i = 0; i < sizeof(v); ++i)
		{
			this->_writeNext(data[i]);
		}

        return *this;
    }

    //FIXME: Portability issue. This could produce invalid results if target platform uses different floating point format than the advised IEEE 754
	template <>
    MGFDataWriter &MGFDataWriter::operator << <double>(const double &d)
    {
	    union
		{
			double v;
			char data[sizeof(v)];  // evil!
		};
		
		v = d;

		for(uint8_t i = 0; i < sizeof(v); ++i)
		{
			this->_writeNext(data[i]);
		}

        return *this;
    }
	
	// is this really a primitive?
	template <>
	MGFDataWriter &MGFDataWriter::operator << <String>(const String &s)
	{
 		*this << static_cast<uint32_t>(s.length());

		_writeChars(s.c_str(), s.length());

		*this << static_cast<uint8_t>(0);

		return *this;
    }
	
	
	
	
	//---- begin readers ----
    
    /* specialization for all integral types (hopefully!)
            NOPE, doesn't work!
    template <typename T>
	typename std::enable_if<std::is_integral<T>::value, MGFDataReader&>::type MGFDataReader::operator >> <T>(T &v)
	{
		_stupidlyReadIntegral<T>(v);

		return *this;
	}*/
    
    template <>
	MGFDataReader &MGFDataReader::operator >> <uint64_t>(uint64_t &l)
	{
		_stupidlyReadIntegral<uint64_t>(l);

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <uint32_t>(uint32_t &i)
	{
		_stupidlyReadIntegral<uint32_t>(i);

		return *this;
	}

    template <>
    MGFDataReader &MGFDataReader::operator >> <uint16_t>(uint16_t &s)
	{
		_stupidlyReadIntegral<uint16_t>(s);

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <uint8_t>(uint8_t &b)
	{
		b = _getNext();

		return *this;
	}
	
	template <>
	MGFDataReader &MGFDataReader::operator >> <int64_t>(int64_t &l)
	{
		_stupidlyReadIntegral<int64_t>(l);

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <int32_t>(int32_t &i)
	{
		_stupidlyReadIntegral<int32_t>(i);

		return *this;
	}

    template <>
    MGFDataReader &MGFDataReader::operator >> <int16_t>(int16_t &s)
	{
		_stupidlyReadIntegral<int16_t>(s);

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <int8_t>(int8_t &b)
	{
		b = _getNext();

		return *this;
	}
    
    //FIXME: Portability issue. This could produce invalid results if target platform uses different floating point format than the advised IEEE 754
    template <>
	MGFDataReader &MGFDataReader::operator >> <float>(float &f)
	{
		union
		{
			float v;
			char data[sizeof(v)];  // evil!
		};

		for(uint8_t i = 0; i < sizeof(v); ++i)
		{
			data[i] = _getNext();
		}

		f = v;

		return *this;
	}

	//FIXME: Portability issue. This could produce invalid results if target platform uses different floating point format than the advised IEEE 754
    template <>
	MGFDataReader &MGFDataReader::operator >> <double>(double &d)
	{
		union
		{
			double v;
			char data[sizeof(v)];  // evil!
		};

		for(uint8_t i = 0; i < sizeof(v); ++i)
		{
			data[i] = _getNext();
		}

		d = v;

		return *this;
	}
    
    // is this really a primitive?
    template <>
	MGFDataReader &MGFDataReader::operator >> <String>(String &s)
	{
		uint32_t len;
		*this >> len;
		char *buf = new char[len];

		for(uint32_t i = 0; i < len; ++i)
		{
		    uint8_t c = _getNext();

		    if(c == 0)
		    {
		        Logger::warn("Premature termination of string. This indicates an invalid lnegth field or malformed MGF");
		    }

			buf[i] = c;
		}

		if(_getNext() != 0)
		{
		    Logger::warn("Missing termination of string. This indicates an invalid lnegth field or malformed MGF");

		    mStream->unget();
		}

		s = String(buf, len);

		delete[] buf;

		return *this;
	}
    
    
    
    
    
    
    /*
    template <>
	MGFDataWriter &MGFDataWriter::operator << <uint64_t>(const uint64_t &l)
	{
		_stupidlyWriteIntegral<uint64_t>(l);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint32_t>(const uint32_t &i)
	{
		_stupidlyWriteIntegral<uint32_t>(i);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint16_t>(const uint16_t &s)
	{
		_stupidlyWriteIntegral<uint16_t>(s);

		return *this;
	}

	template <>
	MGFDataWriter &MGFDataWriter::operator << <uint8_t>(const uint8_t &b)
	{
		_writeNext(b);

		return *this;
	}

	template <>
    MGFDataWriter &MGFDataWriter::operator << <float>(const float &f)
    {
	    // TODO: Implement proper float storage
	    for(uint8_t i = 0; i < sizeof(float); ++i)
	    {
	        _writeNext(0);
	    }

        return *this;
    }

	template <>
    MGFDataWriter &MGFDataWriter::operator << <double>(const double &f)
    {
	    // TODO: Implement proper double storage
        for(uint8_t i = 0; i < sizeof(double); ++i)
        {
            _writeNext(0);
        }

        return *this;
    }
    
    
    // ------ begin readers --------
    
    
    template <>
	MGFDataReader &MGFDataReader::operator >> <uint64_t>(uint64_t &l)
	{
		_stupidlyReadIntegral<uint64_t>(l);

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <uint32_t>(uint32_t &i)
	{
		_stupidlyReadIntegral<uint32_t>(i);

		return *this;
	}

    template <>
    MGFDataReader &MGFDataReader::operator >> <uint16_t>(uint16_t &s)
	{
		_stupidlyReadIntegral<uint16_t>(s);

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <uint8_t>(uint8_t &b)
	{
		b = _getNext();

		return *this;
	}

	//FIXME: Portability issue. This could fail if target platform uses different floating point format than the advised IEEE 754
    template <>
	MGFDataReader &MGFDataReader::operator >> <float>(float &f)
	{
		union
		{
			float v;
			char data[sizeof(v)];
		};

		for(uint8_t i = 0; i < sizeof(v); ++i)
		{
			data[i] = _getNext();
		}

		f = v;

		return *this;
	}

	//FIXME: Portability issue. This could fail if target platform uses different floating point format than the advised IEEE 754
    template <>
	MGFDataReader &MGFDataReader::operator >> <double>(double &d)
	{
		union
		{
			double v;
			char data[sizeof(v)];
		};

		for(uint8_t i = 0; i < sizeof(v); ++i)
		{
			data[i] = _getNext();
		}

		d = v;

		return *this;
	}

    template <>
	MGFDataReader &MGFDataReader::operator >> <String>(String &s)
	{
		uint32_t len;
		*this >> len;
		char *buf = new char[len];

		for(uint32_t i = 0; i < len; ++i)
		{
		    uint8_t c = _getNext();

		    if(c == 0)
		    {
		        Logger::warn("Premature termination of string. This indicates an invalid lnegth field or malformed MGF");
		    }

			buf[i] = c;
		}

		if(_getNext() != 0)
		{
		    Logger::warn("Missing termination of string. This indicates an invalid lnegth field or malformed MGF");

		    mStream->unget();
		}

		s = String(buf, len);

		delete[] buf;

		return *this;
	}
    
    */
    
    
    
}
