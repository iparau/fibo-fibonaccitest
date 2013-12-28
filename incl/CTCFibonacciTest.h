#pragma once

namespace test {
	class CTCFibonacciTest {

	public: 
		CTCFibonacciTest();
		virtual ~CTCFibonacciTest();

	public:
		bool runTests();

	protected: 
		bool isInitialized() { return this->bInitialized; };

	protected:
		bool bInitialized;
	};
};
