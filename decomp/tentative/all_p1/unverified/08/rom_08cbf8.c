/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08cbf8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8cbf8(void)

{
  short sVar1;
  ushort uVar2;
  int unaff_A6;

  if (*(char *)(*(int *)(unaff_A6 + 0x82) + 1) != '\0') {
    sVar1 = 7;
    uVar2 = 0;
    do {
      if (unaff_A6 == *(int *)((short)uVar2 + 0x10c5c2)) {
        *(undefined4 *)((short)uVar2 + 0x10c5c2) = 0;
        *(undefined1 *)((short)(uVar2 >> 2) + 0x10c5e2) = 0;
        return;
      }
      uVar2 = uVar2 + 4;
      sVar1 = sVar1 + -1;
    } while (sVar1 != -1);
  }
  return;
}
