using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NPC : Interactable
{
    public string[] dialogue;
    public string name;

    void Start()
    {
        Color GFXColor = new Color(Random.value, Random.value, Random.value);
        Color helmetColor = new Color(Random.value, Random.value, Random.value);
        this.transform.Find("GFX").GetComponent<Renderer>().material.color = GFXColor;
        this.transform.Find("helmet").GetComponent<Renderer>().material.color = helmetColor;
    }

    public override void Interact()
    {
        base.Interact();
        DialogueSystem.instance.AddNewDialogue(dialogue, name);


    }
}
