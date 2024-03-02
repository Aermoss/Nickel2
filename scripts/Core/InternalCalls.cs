using System;
using System.Runtime.CompilerServices;

namespace Nickel2 {
    public static class InternalCalls {
        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);
        #endregion
    }
}