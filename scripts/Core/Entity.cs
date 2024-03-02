using System;
using System.Runtime.CompilerServices;

namespace Nickel2 {
    public class Entity {
        protected Entity() { id = 0; } 
        internal Entity(ulong id) { this.id = id; }

        public readonly ulong id;

        /* public Vector3 Translation {
            get {
                InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 result);
                return result;
            } set {
                InternalCalls.TransformComponent_SetTranslation(ID, ref value);
            }
        } */
    }
}