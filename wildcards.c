int match_wildcards(char *wildcards, char *filename)
{
    int len;
    int wildcards;
    int file_len;
    if (pattern[0] == '*')
    {
        wildcards_len = ft_strlen(wildcards + 1);
        file_len = ft_strlen(filename);
        if (file_len < pattern_len)
            return 0;
        return ft_strcmp(filename + file_len - pattern_len, pattern + 1) == 0;
    }
    // Cas 2: "test*" - vérifie si le fichier commence par "test"
    else if (pattern[ft_strlen(pattern) - 1] == '*')
    {
        len = ft_strlen(pattern) - 1;// Longueur avant *
        return ft_strncmp(filename, pattern, len) == 0;
    }
    // Cas 3: "test*.c" - vérifier début et fin
    else
    {
        // Trouver la position du '*'
        char *star_pos = strchr(pattern, '*');
        if (star_pos)
        {
            int prefix_len = star_pos - pattern;  // Longueur de la partie avant '*'
            char *suffix = star_pos + 1;          // Partie après '*'
            int suffix_len = strlen(suffix);
            int filename_len = strlen(filename);
            // Vérifier que le fichier est assez long
            if (filename_len < prefix_len + suffix_len)
                return 0;  // Trop court, pas de correspondance
            
            // Vérifier le début et la fin
            if (strncmp(filename, pattern, prefix_len) == 0 &&
                strcmp(filename + filename_len - suffix_len, suffix) == 0)
            {
                return 1;  // Correspondance trouvée
            }
        }
    }
    return (0);  // Par défaut, pas de correspondance
}

char **collect_files(char *pattern)
{
    struct dirent;
    int i = 0;
    int arg_wildcards = 0;
    DIR *dir;
    char **name_wildcards;
    dir = opendir(".");
    if (dir == NULL)
    {
        perror("opendir");
        return NULL;
    }
    dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (match_wildcards(pattern, entry->d_name) == 1)
            arg_wildcards++;
    }
    closedir(dir);
    name_wildcards = malloc(sizeof(char*) * (arg_wildcards + 1));
    if (!name_wildcards)
        return NULL;
    dir = opendir(".");
    if (dir == NULL)
    {
        free(name_wildcards);
        perror("opendir");
        return NULL;
    }
    i = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (match_wildcards(pattern, entry->d_name) == 1)
        {
            name_wildcards[i] = ft_strdup(entry->d_name);
            i++;
        }
    }
    name_wildcards[i] = NULL; // Terminer le tableau par NULL
    closedir(dir);
    return (name_wildcards);
}
int	exexcmd_wildcards(t_cmd *cmd, char*pattern, char **env)
{
	int i = 0;
	char **name_wildcards = collect_files(char *pattern);
	while (name_wildcards[i])
	{
		execve(shell->cmd_paths, name_wildcards[i],env);
	}
}
