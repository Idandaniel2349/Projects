using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerManager : MonoBehaviour
{
    public static PlayerManager instance;

    public CharactersStats myStats;

    public GameObject player;

    void Awake()
    {
        instance = this;
    }

    void Start()
    {
        myStats = player.GetComponent<CharactersStats>();
    }

    public void killPlayer()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
    }

    public int AddHealth(int healthUp)
    {
        return myStats.HealthUp(healthUp);
    }

    
    public void AddExp(int expAquired)
    {
        myStats.ExpUp(expAquired);
    }
    


}
