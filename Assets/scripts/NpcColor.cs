using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NpcColor : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Color GFXColor = new Color(Random.value, Random.value, Random.value);
        this.transform.Find("GFX").GetComponent<Renderer>().material.color = GFXColor;
    }
}
