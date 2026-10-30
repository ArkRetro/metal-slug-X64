/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae852.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x000aa890) */

void candidate_ae852(void)

{
  code *pcVar1;
  byte bVar2;
  int iVar3;
  int unaff_A5;
  int unaff_A6;

  bVar2 = (byte)*(undefined2 *)(unaff_A6 + 0x76);
  if ((char)*(undefined2 *)(unaff_A6 + 0x6e) == '\0') {
    iVar3 = unaff_A5 + 0x4608;
    pcVar1 = candidate_42e8;
  }
  else {
    iVar3 = 0x10c62c;
    pcVar1 = thunk_FUN_000044bc;
  }
  *(undefined2 *)(iVar3 + 0x18) = *(undefined2 *)(unaff_A6 + 0x4e);
  *(undefined2 *)(iVar3 + 0x16) = 0;
  if (99 < bVar2) {
    bVar2 = 99;
  }
  *(undefined1 *)(iVar3 + 0x21) = 4;
  *(byte *)(iVar3 + 0x1f) = bVar2;
  *(undefined1 *)(iVar3 + 0x20) = 6;
  *(undefined2 *)(iVar3 + 0x14) = 0x30;
  *(undefined4 *)(pcVar1 + unaff_A5) = 0x16dc6;
  return;
}
