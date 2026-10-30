/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x004624.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x0000466e: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x00004672) */

void candidate_4624(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined4 *puVar3;
  int unaff_A5;

  *(undefined1 *)(unaff_A5 + -0x181e) = 0;
  *(undefined1 *)(unaff_A5 + -0x181d) = 0;
  *(undefined1 *)(unaff_A5 + -0x181c) = 0;
  *(undefined1 *)(unaff_A5 + -0x181b) = 0;
  puVar3 = (undefined4 *)0x106756;
  do {
    *puVar3 = 0;
    puVar3[1] = 0;
    puVar3 = puVar3 + 2;
  } while (puVar3 < (undefined4 *)0x1067d6);
  sVar1 = 0xff;
  *(undefined1 **)(unaff_A5 + -0x171a) = (undefined1 *)(unaff_A5 + -0x181a);
  puVar2 = (undefined1 *)(unaff_A5 + -0x181a);
  do {
    *puVar2 = 0;
    sVar1 = sVar1 + -1;
    puVar2 = puVar2 + 1;
  } while (sVar1 != -1);
  *(undefined2 *)(unaff_A5 + -0x1b1c) = 0;
  *(undefined4 *)(unaff_A5 + -0x1a7a) = 0;
  *(undefined4 *)(unaff_A5 + -0x1a76) = 0;
  *(undefined4 *)(unaff_A5 + -0x1a72) = 0;
  *(undefined4 *)(unaff_A5 + -0x1a6e) = 0;
  return;
}
