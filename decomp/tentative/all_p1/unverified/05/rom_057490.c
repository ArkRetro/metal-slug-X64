/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x057490.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_57490(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x72) = 0xf;
  *unaff_A6 = 0x5749c;
  sVar3 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar3;
  if (sVar3 != 0) {
    return;
  }
  candidate_572ce();
  candidate_3cfba();
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x1e;
  *unaff_A6 = 0x574e6;
  sVar3 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar3;
  if (sVar3 != 0) {
    return;
  }
  candidate_40bc();
  *(undefined2 *)(extraout_A0 + 0x74) = *(undefined2 *)(unaff_A6 + 0x1d);
  *(undefined2 *)(extraout_A0 + 0x78) = *(undefined2 *)(unaff_A6 + 0x1e);
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x57516;
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    return;
  }
  *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
