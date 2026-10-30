/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa83a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa83a(void)

{
  code *pcVar1;
  char in_D0b;
  char in_D1b;
  byte unaff_D2b;
  undefined1 unaff_D3b;
  undefined2 unaff_D4w;
  int iVar2;
  int unaff_A5;
  int unaff_A6;

  if (in_D0b == '\0') {
    iVar2 = unaff_A5 + 0x4608;
    pcVar1 = candidate_42e8;
  }
  else {
    iVar2 = 0x10c62c;
    pcVar1 = thunk_FUN_000044bc;
  }
  *(undefined2 *)(iVar2 + 0x18) = *(undefined2 *)(unaff_A6 + 0x4e);
  *(undefined2 *)(iVar2 + 0x16) = 0;
  if (99 < unaff_D2b) {
    unaff_D2b = 99;
  }
  *(char *)(iVar2 + 0x21) = in_D1b;
  *(byte *)(iVar2 + 0x1f) = unaff_D2b;
  *(undefined1 *)(iVar2 + 0x20) = unaff_D3b;
  *(undefined2 *)(iVar2 + 0x14) = unaff_D4w;
  if (in_D1b == '\x04') {
    *(undefined4 *)(pcVar1 + unaff_A5) = 0x16dc6;
  }
  else {
    *(undefined4 *)(pcVar1 + unaff_A5) = 0x16dbe;
  }
  return;
}
