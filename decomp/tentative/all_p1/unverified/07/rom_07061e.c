/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07061e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7061e(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined1 *)((int)unaff_A6 + 0x86) = 0xff;
  *(undefined2 *)(unaff_A6 + 0x1f) = 0xffff;
  candidate_1ad6e();
  *unaff_A6 = 0x7063a;
  bVar3 = false;
  candidate_409a();
  if (!bVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1aca2();
  if (*(char *)(extraout_A0 + 0x86) < '\0') {
    return;
  }
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(extraout_A0 + 0x1e);
  *(undefined2 *)((int)unaff_A6 + 0x7a) = *(undefined2 *)(extraout_A0 + 0x7a);
  *(undefined1 *)((int)unaff_A6 + 0x87) = *(undefined1 *)(extraout_A0 + 0x87);
  if (*(short *)(unaff_A6[3] + 0x7c) != *(short *)(unaff_A6 + 0x1f)) {
    *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6[3] + 0x7c);
    candidate_9f88();
    candidate_9ea8();
  }
  if (*(char *)(unaff_A6[3] + 0x86) != *(char *)((int)unaff_A6 + 0x86)) {
    *(char *)((int)unaff_A6 + 0x86) = *(char *)(unaff_A6[3] + 0x86);
    candidate_4256();
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
