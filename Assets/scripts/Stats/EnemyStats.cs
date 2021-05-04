using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EnemyStats : CharactersStats
{
    public GameObject healthBarUI;
    public Slider silder;

    public static PlayerManager playerManager;
    public int expWorth;

    public GameObject potion;
    private Vector3 potionPosition;
    private Quaternion potionRotation;

    void Start()
    {
        playerManager = PlayerManager.instance;
        expWorth = 10;
    }
   

    public override void takeDamage(int damage)
    {
        base.takeDamage(damage);
        AudioManager.instance.Play("EnemyHit");
        silder.value = (float)currentHealth / (float)maxHealth;
        if (currentHealth <= 0)
        {
            Die();
            return;
        }
        //play hit sound if regular hit
        AudioManager.instance.Play("EnemyHit");
    }

    public override void Die()
    {
        base.Die();
        AudioManager.instance.Play("EnemyDeath");
        //death animation
        Drop();
        Destroy(gameObject);
        playerManager.AddExp(expWorth);
    }

    public void Drop()
    {
        float random = Random.Range(-1f, 1f);
        if (random > 0)
        {
            potionPosition.x = this.transform.position.x;
            potionPosition.y = this.transform.position.y+1;
            potionPosition.z = this.transform.position.z;
            potionRotation = Quaternion.Euler(-90, 0, 0);

            Instantiate(potion, potionPosition , potionRotation);
        }
    }
}
