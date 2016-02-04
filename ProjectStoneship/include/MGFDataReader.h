
#ifndef _MGFDATAREADER_H
#define _MGFDATAREADER_H

#include <istream>

#include "Types.h"
#include "Exception.h"

namespace Stoneship
{

    class MasterGameFile;

	class MGFDataReader
	{
        class endr_t{};

	public:

		MGFDataReader(std::istream *stream, MasterGameFile *mgf = nullptr, uint32_t unitSize = 0);

		MasterGameFile *getMasterGameFile();
		std::istream *getStream();

		//adapters for seekg/tellg with additional error checking and EOF handling
		MGFDataReader &seek(std::streampos pos);
		std::streampos tell();

		template <typename T>
		MGFDataReader &operator>>(T &s);
		MGFDataReader &operator>>(endr_t);

		void beginUnit(uint32_t size);
		void endUnit();
		uint32_t bytesRemainingInUnit();

		void skip(uint32_t amount);
		void skipToEnd();

		bool hasEnded();


		static uint32_t SEEKS;
		static uint32_t TELLS;
		static uint32_t GETCS;

		static const endr_t endr;

	private:

		void _readChars(char* data, uint32_t size);

		template <typename T>
        void _stupidlyReadIntegral(T &v)
        {
            union{ T vt; uint8_t vb[sizeof(T)];} vu;

            for(uint8_t i = 0; i < sizeof(T); ++i)
            {
                //TODO: find a better way to convert to little endian. this might be platform dependent

                vu.vb[i] = _getNext();
            }

            v = vu.vt;
        }

		void _checkBounds(std::streamoff off);

		void _except(StoneshipException::ExceptionType type, const String &msg);

		uint8_t _getNext();

		std::istream *mStream;
		MasterGameFile *mGameFile;

		std::streamoff mUnitStart;
		std::streamoff mUnitEnd;
		bool mFinite;
	};

}


#endif
