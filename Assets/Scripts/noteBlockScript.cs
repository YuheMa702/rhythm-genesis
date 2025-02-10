using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class noteController : MonoBehaviour
{

    void Start(){
        // Vector2 position = transform.position;
        // position.y = 6f;
        // int x = Random.Range(-2, 3);
        // position.x = x * 5;
        // transform.position = position;
    }

    void Update(){
        Vector2 position = transform.position;
        if(position.y < -6.0f){
            Destroy(gameObject);
        }
        position.y = position.y - 0.01f;
        if (Input.GetKeyDown(KeyCode.Space)){
            position.y = position.y + 0.2f;
        }else if (Input.GetKeyDown(KeyCode.E)){
            position.y = 6f;
            int x = Random.Range(-2, 3);
            position.x = x * 5;
        }
        transform.position = position;
    }

   
}
