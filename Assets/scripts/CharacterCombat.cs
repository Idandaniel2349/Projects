using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CharactersStats))]
public class CharacterCombat : MonoBehaviour
{

    public float attackSpeed=1f;
    private float attaclCooldown = 0f;
    public float attaclDelay = .6f;
    CharactersStats myStats;

    public event System.Action OnAttack;

    void Start()
    {
        myStats=GetComponent<CharactersStats> ();
    }

    void Update()
    {
        attaclCooldown -= Time.deltaTime;
    }
    public void Attack(CharactersStats targetStats)
    {
        if (attaclCooldown <= 0f) 
        {

            StartCoroutine(DoDamage(targetStats, attaclDelay));

            if (OnAttack != null)
            {
                OnAttack();
            }
            attaclCooldown=1f / attackSpeed;
        }
      
    }

    IEnumerator DoDamage (CharactersStats stats, float delay)
    {
        yield return new WaitForSeconds(delay);

        stats.takeDamage(myStats.damage.GetValue());
    }
}
