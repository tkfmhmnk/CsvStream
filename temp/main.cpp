#include<iostream>
#include<type_traits>
#include"Header.h"


#include <utility>

template <class... Args>
struct is_callable_impl {
	template <class F>
	static std::true_type
		check(decltype(std::declval<F>()(std::declval<Args>()...), (void)0)*);

	template <class F>
	static std::false_type check(...);
};

template <class F, class... Args>
struct is_callable
	: decltype(is_callable_impl<Args...>::template check<F>(nullptr)) {};



template<int _base>
class num_traits {
public:
	static constexpr int base = _base;
};


template<class CharT,class NumT,class NumTraits = num_traits<10>>
NumT strtonum(const CharT* str, const CharT** end_ptr) {
	if constexpr (std::is_same_v<NumT, long>) {
		return strtol(str, (CharT**)end_ptr, NumTraits::base);
	}
	else {
		return NumT();
	}
}

class extclass {
public:
	long val1;
	long val2;
};

//namespace CsvLoader {
//	template<class CharT, class... TailT>
//	const CharT* Load_ExtClass(const CharT* str, extclass& obj, TailT&... tail) {
//		return Load(str, obj.val1, Hex(obj.val2), tail...);
//	}
//}

namespace CsvLoader {

	class DefaultItemTraits {
	public:
		static constexpr int numeric_base = 10;
	};

	template<class T>
	class Dec {
	public:
		Dec(T& _des) : des(_des) {

		}
		using NumTraits = num_traits<10>;
		using NumT = T;
	protected:
		T& des;
	};

	template<class T>
	class Hex {
	public:
		Hex(T& _des) : des(_des) {

		}
		using NumTraits = num_traits<16>;
		using NumT = T;
	protected:
		T& des;
	};

	template<class CharT, class... TailT>
	const CharT* Load_ExtClass(const CharT* str, extclass& obj, TailT&... tail) {
		//return Load(str, obj.val1, Hex(obj.val2), tail...);[
		return nullptr;
	}

	void Load_ExtClass2(extclass& obj) {
		//return Load(str, obj.val1, Hex(obj.val2), tail...);[
		return;
	}


	template<class CharT,class T,class...Tail>
	const CharT* testfunc(const CharT* str,T& a,Tail&... tail){

		return nullptr;
	}

	namespace detail {

		template < class T, class U1 = typename T::NumTraits, class U2 = typename T::DesT>
		std::true_type has_NumTraits_NumT(int);
		template < class T >
		std::false_type has_NumTraits_NumT(long);


		template<class CharT, class _DesT, class DesTraits = Dec<_DesT>>
		const CharT* _Load(const CharT* str, _DesT& des) {
			return str;
		}


		template<class CharT, class _DesT, class DesTraits = Dec<_DesT>, class... TailT>
		const CharT* _Load(const CharT* str,_DesT& des,TailT&... tail) {
			using DesT = std::remove_reference_t<_DesT>;
			//Load_ExtClass<CharT, DesT&>(str, des, tail...);
			//Load_ExtClass<CharT, DesT>(str, des);
			//Load_ExtClass(str, des,tail...);
			Load_ExtClass2(des);

			if constexpr (std::is_invocable_v<decltype(Load_ExtClass2), std::add_lvalue_reference_t<DesT>>) {


				if constexpr (std::is_invocable_v<decltype(testfunc<CharT, DesT&>), const CharT*, DesT&>) {
					testfunc<CharT, DesT&>(str, des, tail...);
					int a = 0;
					if constexpr (std::is_same_v<decltype(has_NumTraits_NumT<_DesT>(0)), std::true_type>) {
						return _Load<CharT, typename DesT::NumT, typename DesT::DesTraits>(str, des.des, tail...);
					}
					else if constexpr (std::is_invocable_v<decltype(Load_ExtClass<CharT>),const CharT*, std::add_lvalue_reference_t<DesT>>) {
						return Load_ExtClass<CharT>(str, des, tail...);
						//return nullptr;
					}
					else {
						const CharT* end = nullptr;
						des = strtonum<CharT, DesT, typename DesTraits::NumTraits>(str, &end);

						return _Load(++end, tail...);
					}
				}

			}
		}

		//_LoadElem() {

		//}




	}






	/**
	root.ItemTraitsは指定させない
*/
	template<class CharT, class _DesT, class... TailT>
	const CharT* Load(const CharT* str, _DesT& des, TailT&... tail) {
		return detail::_Load(str, des, tail...);
	}



}

//namespace CsvLoader {
//	template<class CharT, class... TailT>
//	const CharT* Load_ExtClass(const CharT* str, extclass& obj, TailT&... tail) {
//		return Load(str, obj.val1, Hex(obj.val2), tail...);
//	}
//}




template<class DesT,class ItemTraits = double , class... Tail>
void func(DesT&& des, Tail&&... tail) {
	ItemTraits val = 0;
}


int main() {
	using namespace std;
	using namespace CsvLoader;
	long val1=0, val2=0;
	long dummy;
	extclass obj;
	Load("1,2", obj,obj);
	return 0;
}




//
//template<class DesT>
//proc(des) {
//	if (配列) {
//		for () {
//			proc(要素)
//		}
//	}
//	if (構造体) {
//		for () {	//外部で定義？
//			proc(要素)
//		}
//	}
//	else {
//		変数変換
//	}
//
//
//}
//
//
//template<class DesT,class Tail>
//proc(des,tail) {
//	if (配列) {
//		for () {
//			proc2(要素)
//		}
//
//		proc(tail)
//	}
//	if (構造体) {
//		return 外部のオーバーロード関数呼び出し
//	}
//	if (進数マニプ) {
//		return proc(要素、tail)
//	}
//	else {
//		proc2(des)
//
//		proc(tail)
//	}
//}
////単一のリテラル型を処理
//proc2(val) {
//	if (数値{
//		des数値変換
//	}
//	if (文字列) {
//
//	}
//}
//
////構造体をメンバに展開してprocを呼び出す。構造体をprocを結び付ける
//template<class Tail>
//外部のオーバーロード関数(構造体,tail) {
//	proc(a, b, c, d,tail);
//}
//
//
//
//
//
//
//






//
//
////tailはほとんどなし。構造体は外部でprocに対してtailをつけてprocを実行する
//template<class DesT,class Tail>
//proc(des,tail) {
//	if (配列) {
//		for () {
//			proc(要素)
//		}
//	}
//	if (構造体) {
//		外部のオーバーロード関数呼び出し
//	}
//	else {
//		変数変換
//	}
//}
//
////構造体をメンバに展開してprocを呼び出す。構造体をprocを結び付ける
//外部のオーバーロード関数(構造体) {
//	proc(a, b, c, d);
//}
