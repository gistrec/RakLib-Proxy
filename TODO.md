# Добавить поддержку нескольких версий (хотя бы для ubuntu 16.04 и 18.04)


## Регестрируем функцию в нетфильтре
`https://unix.stackexchange.com/questions/413797/nf-register-hook-not-found-in-linux-kernel-4-13-rc2-and-later`

```
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    nf_register_net_hook(NULL, &nfho_in)
#else
    nf_register_hook(&nfho_in)
#endif
```

```
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    nf_unregister_net_hook(NULL, &nfho_in)
#else
    nf_unregister_hook(&nfho_in)
#endif
```





## Разные заголовки функции, которая принимает пакет

`https://stackoverflow.com/questions/25124033/what-is-the-correct-way-to-define-a-netfilter-hook-function/44186227#44186227`

