/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c4dd8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c4dd8(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_9ea8();
  candidate_4256();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0x10;
  candidate_1ad6e();
  candidate_120f8();
  *unaff_A6 = 0xc4e1c;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  sVar3 = *(short *)((int)unaff_A6 + 0x6e);
  *(short *)((int)unaff_A6 + 0x6e) = sVar3 + -1;
  if (sVar3 == 0) {
    *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
