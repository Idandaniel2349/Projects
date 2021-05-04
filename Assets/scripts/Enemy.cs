using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CharactersStats))]
public class Enemy : Interactable
{

    PlayerManager playerManager;
    CharactersStats myStats;


    void Start()
    {
        playerManager = PlayerManager.instance;
        myStats = GetComponent<CharactersStats>();
        Color GFXColor = new Color(Random.value, Random.value, Random.value);
        Color helmetColor = new Color(Random.value, Random.value, Random.value);
        this.transform.Find("GFX").GetComponent<Renderer>().material.color = GFXColor;
        this.transform.Find("helmet").GetComponent<Renderer>().material.color = helmetColor;
    }

    public override void Interact()
    {
        base.Interact();
        CharacterCombat playerCombat= playerManager.player.GetComponent<CharacterCombat>();
        if (playerCombat != null)//make sure playerCombat was made
        {
            playerCombat.Attack(myStats);
        }

    }
}
