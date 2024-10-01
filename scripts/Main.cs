using System;
using System.Runtime.CompilerServices;

using Nickel2;

namespace Main {
    public class Test : Entity {
        void OnCreate() {
            // Console.WriteLine(id);
        }

        void OnUpdate(float deltaTime) {
            if (Input.IsKeyDown(KeyCode.V)) {
                Console.WriteLine("deltaTime: " + (string) deltaTime.ToString());
            }
        }
    }
}