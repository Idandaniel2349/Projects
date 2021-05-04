using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "New HeakthPotion", menuName = "HealthPotion")]
public class HelathPotion : Item
{
    PlayerStats playerStats;
    public int healthUp = 20;


    public override void Use()
    {
        base.Use();
        if(PlayerManager.instance.AddHealth(healthUp)==1)
            RemoveFromInventory();//remove from inventory if we used it
    }
}
