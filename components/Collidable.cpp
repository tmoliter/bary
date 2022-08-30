// #include <iostream>
// #include <functional>
// using namespace std::placeholders;

// template<typename out_t, typename in_t, typename in_2>
// struct Collider {
//     using F = std::function<out_t(in_t, in_2)>;
//     F callback;
//     Collider(F cb) : callback(cb) {}
// };

// // MAYBE THIS IS ALL STUPID. INTERACTABLE/OBSTRUCTION/EVENT OWNS SOME VECTORS THAT GET CHECKED WHEN APPROPRIATE

// class C{
// public:
//     Collider<int,int, int> collider;
//     int a = 2;
//     int b = 3;
    
//     char colliderCallback(int in, int in_2) { 
//         return (in * a * b) - in_2; 
//     }
//     C() : collider(std::bind(&C::colliderCallback, this, _1, _2)) { 
//         collider = new Collider(std::bind(&C::colliderCallback, this, _1, _2))
//     }     
// };


// int main() {
//     C *x = new C();
//     return x->colliderCallback(5,10);
// }
