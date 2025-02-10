using UnityEngine;
using System.Runtime.CompilerServices;
using UnityEngine.InputSystem;
using System.Collections;
using System.Collections.Generic;

public class lineScript : MonoBehaviour
{

    void OnTriggerEnter2D(Collider2D collision) => startColl(collision);
    // void OnTriggerStay2D(Collider2D collision) => checkColl(collision);
    void OnTriggerExit2D(Collider2D collision) => endColl(collision);

    List<GameObject> inCollision = new List<GameObject>();

    // void Log(Collider2D collision, [CallerMemberName] string message = null)
    // {
    //     Debug.Log($"{message} called on {name} because a collision with {collision.gameObject.name}");
    // }
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.F)){
            while(inCollision.Count > 0){
                GameObject x = inCollision[0];
                inCollision.Remove(x);
                Destroy(x);
            }
        }
    }
    

    void startColl(Collider2D collision)
    {
        GameObject coll = collision.gameObject;
        inCollision.Add(coll);
    }

    void endColl(Collider2D collision)
    {
        GameObject coll = collision.gameObject;
        inCollision.Remove(coll);
    }

}
