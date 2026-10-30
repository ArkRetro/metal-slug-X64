/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x048982.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_48982(void)

{
  char *pcVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_1ad52();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x96) = 3;
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x96) = 0xb;
  }
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *unaff_A6 = 0x489c8;
  uVar2 = 0;
  candidate_49efa();
  candidate_1ac38();
  candidate_1ad8c();
  candidate_49c82();
  candidate_4a064();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_48a06;
  }
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + '\x03';
  pcVar1 = (char *)(*(int *)(unaff_A6[3] + 0xc) + 0x95);
  *pcVar1 = *pcVar1 + '\x01';
  candidate_49030();
  return;
}
