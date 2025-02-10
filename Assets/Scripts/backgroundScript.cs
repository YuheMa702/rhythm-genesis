using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class backgroundScript : MonoBehaviour
{

    public GameObject Square;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        Instantiate(Square);
    }

    private float nextFire = 5.0f;

    void Update()
    {
        if (Time.time > nextFire)
        {
            int z = Random.Range(0, 5);
            List<float> positions = new List<float>();
            while(positions.Count < z){
                int x = Random.Range(-2, 3) * 5;
                if(positions.Contains(x) == false){
                    positions.Add(x);
                }
            }
            for(int i = 0; i < z; i ++){
                GameObject s = Instantiate(Square);
                Vector2 position = s.transform.position;
                position.y = 6f;
                position.x = positions[i];
                s.transform.position = position;
            }
            nextFire = nextFire + 5.0f;
        }
    }
}
