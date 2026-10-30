/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x037cec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_37cec(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  *(undefined1 *)((int)unaff_A6 + 0x97) = 2;
  *(undefined2 *)(unaff_A6 + 10) = *(undefined2 *)(unaff_A6[3] + 0x28);
  *(undefined2 *)(unaff_A6 + 0xb) = *(undefined2 *)(unaff_A6[3] + 0x2c);
  bVar3 = false;
  candidate_37c78();
  if (!bVar3) {
    candidate_37c94();
    candidate_1ad6e();
    candidate_9ea8();
    candidate_4256();
    *unaff_A6 = 0x37d3e;
    uVar4 = 0;
    if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) == 0) {
      candidate_1abc8();
      candidate_427a();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_37d64;
      }
      return;
    }
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
