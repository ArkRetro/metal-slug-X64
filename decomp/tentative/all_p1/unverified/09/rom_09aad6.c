/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09aad6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_9aad6(void)

{
  int iVar1;
  ushort *puVar2;
  char *pcVar3;
  int unaff_A6;
  undefined1 in_CF;
  undefined2 uVar4;

  uVar4 = 0xaadc;
  iVar1 = candidate_18f24();
  if ((bool)in_CF) {
    return iVar1;
  }
  candidate_1aeb0(uVar4);
  iVar1 = (int)(short)iVar1;
  if ((bool)in_CF) {
    iVar1 = candidate_c8cc2();
    if ((char)iVar1 == '\0') {
      puVar2 = (ushort *)&DAT_0010c5fc;
      pcVar3 = &DAT_0010c5ff;
    }
    else {
      puVar2 = (ushort *)&DAT_0010c600;
      pcVar3 = &DAT_0010c603;
    }
    *puVar2 = *puVar2 + 1;
    if (99 < *puVar2) {
      *puVar2 = 99;
    }
    if (*(short *)(unaff_A6 + 0x74) == -2) {
      *pcVar3 = *pcVar3 + '\x01';
    }
  }
  return iVar1;
}
