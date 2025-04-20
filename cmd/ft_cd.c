#include "minishell.h"


int ft_cd(t_tree_node *root, char **envp)
{
    struct stat buf;
    char old_pwd[PATH_MAX];
    char new_pwd[PATH_MAX];
    char *home;
    char *oldpwd_ptr;

    // Afficher les valeurs initiales
    printf("DEBUG - Initial: PWD=%s, OLDPWD=%s\n", getenv("PWD"), getenv("OLDPWD"));

    if (getcwd(old_pwd, PATH_MAX) == NULL)
    {
        perror("minishell: getcwd");
        return (1);
    }
    printf("DEBUG - old_pwd=%s\n", old_pwd);

    // Cas: cd -
    if (root->cmd->args[1] && root->cmd->args[1][0] == '-' && root->cmd->args[1][1] == '\0')
    {
        oldpwd_ptr = getenv("OLDPWD");
        printf("DEBUG - OLDPWD=%s\n", oldpwd_ptr ? oldpwd_ptr : "NULL");
        
        if (oldpwd_ptr == NULL)
        {
            write(2, "minishell: cd: OLDPWD not set\n", 30);
            return 1;
        }
        
        // Sauvegarder l'ancien chemin AVANT de changer
        char temp_pwd[PATH_MAX];
        strcpy(temp_pwd, old_pwd);
        printf("DEBUG - temp_pwd=%s\n", temp_pwd);
        
        if (chdir(oldpwd_ptr) != 0)
        {
            perror("minishell: cd");
            return (1);
        }
        
        printf("%s\n", oldpwd_ptr);
        
        // Vérifier le nouveau répertoire
        if (getcwd(new_pwd, PATH_MAX) != NULL)
        {
            printf("DEBUG - new_pwd=%s\n", new_pwd);
            printf("DEBUG - Mise à jour: PWD=%s, OLDPWD=%s\n", new_pwd, temp_pwd);
            
            // Problème potentiel ici - vérifiez si ces fonctions modifient réellement l'environnement
            set_env_var(&envp, "PWD", new_pwd);
            set_env_var(&envp, "OLDPWD", temp_pwd);
            
            // Vérifier après mise à jour
            printf("DEBUG - Après mise à jour: PWD=%s, OLDPWD=%s\n", getenv("PWD"), getenv("OLDPWD"));
        }
        
        return (0);
    }
    // Cas 3: cd avec un chemin spécifique
    else
    {
        if (chdir(root->cmd->args[1]) != 0)
        {
            perror("minishell: cd");
            return (1);
        }
    }

    // Mettre à jour les variables d'environnement PWD et OLDPWD après changement
    // (exécuté seulement pour cd sans argument ou cd avec chemin spécifique)
    if (getcwd(new_pwd, PATH_MAX) != NULL)
    {
        set_env_var(&envp, "PWD", new_pwd);
        set_env_var(&envp, "OLDPWD", old_pwd);
    }

    return (0);
}
