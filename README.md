# Hexy Engine
Hexy Engine is a game engine with editor that uses Mono for scripting.

- [X] Rendering abstraction (2D, 3D, materials, lighting)
- [X] Editor
  - [X] Building standalone executable
  - [X] Play mode
- [X] C# scripting

Editor Screenshot
![Screenshot_11](https://user-images.githubusercontent.com/31342770/178994585-bf2db5be-3efc-45ee-b6aa-1f8d9bacd328.jpg)


Example C# script
```C#
using System;
using System.Collections.Generic;
using Hexy;

namespace ScriptCustom
{
    class TestScript : Entity
    {
        public float speed = 3.0f;
        public void OnStart()
        {
            Console.WriteLine("Hello im OnStart() Function");
            
            var entities = FindEntitiesByName("Empty Entity");
            
            int count = 0;
            foreach(Entity entity in entities)
            {
                entity.GetComponent<NameComponent>().Name += "(" + count + ")";
                count++;
            }

            var mesh = GetComponent<MeshComponent>().mesh;

            var material = mesh.GetMaterial();
            material.Set("u_Albedo", new Vector3(1));
            material.Set("u_useAlbedoTexture", 0.0f);
            //material.Set("u_AlbedoTexture", new Texture2D("assets/textures/texture2.jpg"));
        }

        public void OnUpdate(float deltaTime)
        {
            TransformComponent comp = GetComponent<TransformComponent>();

            Vector3 translation = new Vector3(0);
            if (Input.IsKeyDown('A'))
                translation.x -= speed;
            if (Input.IsKeyDown('D'))
                translation.x += speed;
            if (Input.IsKeyDown('W'))
                translation.y += speed;
            if (Input.IsKeyDown('S'))
                translation.y -= speed;

            comp.position = comp.position + translation * deltaTime;
        }
    }
}
```
