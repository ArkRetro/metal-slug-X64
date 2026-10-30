/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa9e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa9e4(void)

{
  short sVar1;
  char in_D0b;
  int iVar2;
  int unaff_A5;
  int unaff_A6;
  bool bVar3;

  if (in_D0b == '\0') {
    iVar2 = unaff_A5 + 0x4608;
  }
  else {
    iVar2 = unaff_A5 + 0x462c;
  }
  bVar3 = false;
  candidate_4c24();
  if (bVar3) {
    if (*(char *)(unaff_A6 + 0x5a) == '\x0f') {
      *(undefined2 *)(iVar2 + 0x18) = 0;
      *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x80;
      goto LAB_000aaa74;
    }
    if (*(char *)(unaff_A6 + 0x5a) == '\r') {
      *(undefined2 *)(iVar2 + 0x18) = *(undefined2 *)(unaff_A6 + 0x4e);
    }
    else {
      *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0x7f;
      if ((*(byte *)(unaff_A6 + 0x5f) & 0x40) == 0) {
        sVar1 = *(ushort *)(iVar2 + 0x18) - 0x10;
        if (*(ushort *)(iVar2 + 0x18) < 0x10 || sVar1 == 0) {
          *(undefined2 *)(iVar2 + 0x18) = 0;
          *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x80;
          goto LAB_000aaa74;
        }
        *(short *)(iVar2 + 0x18) = sVar1;
      }
    }
  }
  sVar1 = *(short *)(iVar2 + 0x16);
  *(undefined2 *)(iVar2 + 0x16) = 0;
  *(short *)(iVar2 + 0x18) = sVar1 + *(short *)(iVar2 + 0x18);
LAB_000aaa74:
  *(undefined2 *)(unaff_A6 + 0x4e) = *(undefined2 *)(iVar2 + 0x18);
  return;
}
