/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0549b8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_549b8(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  if (*(char *)(unaff_A5 + 0x444e) != '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x69) = *(undefined1 *)(unaff_A5 + 0x444f);
    candidate_c7b48();
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 0x14;
    *unaff_A6 = 0x549dc;
    cVar3 = *(char *)((int)unaff_A6 + 0x8a) + -1;
    *(char *)((int)unaff_A6 + 0x8a) = cVar3;
    if (cVar3 < '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
      *(undefined1 *)(unaff_A6 + 0x24) = 0;
      *unaff_A6 = 0x549f4;
      if ((*(char *)(unaff_A6 + 0x18) == '\0') ||
         ((*(char *)(unaff_A6 + 0x24) == '\a' &&
          (*unaff_A6 = 0x54a0c, *(char *)(unaff_A6 + 0x18) == '\0')))) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
        *(undefined1 *)((int)unaff_A6 + 0x8a) = 0x1e;
        *unaff_A6 = 0x54a2e;
        cVar3 = *(char *)((int)unaff_A6 + 0x8a);
        *(char *)((int)unaff_A6 + 0x8a) = cVar3 + -1;
        if (cVar3 == '\0') {
          candidate_c8cd6();
          candidate_ca2e0();
          candidate_ca2e0();
          candidate_1da40();
          candidate_120f8();
          *(undefined1 *)((int)unaff_A6 + 0x8a) = 0x32;
          *unaff_A6 = 0x54a76;
          cVar3 = *(char *)((int)unaff_A6 + 0x8a);
          *(char *)((int)unaff_A6 + 0x8a) = cVar3 + -1;
          if (cVar3 == '\0') {
            candidate_40bc();
            candidate_c801a();
            candidate_4bfe();
            *(undefined1 *)((int)unaff_A6 + 0x91) = 0xff;
            candidate_1da40();
            *(undefined2 *)(extraout_A0 + 0x66) = 0xffed;
            *(undefined2 *)(extraout_A0 + 0x1e) = 0xffed;
            *(undefined1 *)((int)unaff_A6 + 0x8a) = 0x32;
            *unaff_A6 = 0x54abe;
            cVar3 = *(char *)((int)unaff_A6 + 0x8a);
            bVar4 = cVar3 == '\0';
            *(char *)((int)unaff_A6 + 0x8a) = cVar3 + -1;
            if (bVar4) {
              candidate_552cc();
              if (bVar4) {
                candidate_19dfc();
                candidate_c7b48();
                candidate_ca2f2();
                candidate_ca2f2();
                candidate_c8ce6();
                *(undefined1 *)((int)unaff_A6 + 0x8a) = 100;
                *unaff_A6 = 0x54b0a;
                cVar3 = *(char *)((int)unaff_A6 + 0x8a) + -1;
                *(char *)((int)unaff_A6 + 0x8a) = cVar3;
                if (cVar3 < '\0') {
                  candidate_4192();
                  candidate_4126();
                  iVar2 = *(int *)(unaff_A5 + -0x1c24);
                  do {
                    unaff_A6 = (undefined4 *)unaff_A6[2];
                    pcVar1 = (code *)*unaff_A6;
                    *(undefined4 *)(iVar2 + -4) = 0x424a;
                    (*pcVar1)();
                  } while( true );
                }
              }
              else {
                *unaff_A6 = 0x54ac6;
              }
            }
          }
        }
      }
    }
  }
  candidate_1abc8();
  return;
}
