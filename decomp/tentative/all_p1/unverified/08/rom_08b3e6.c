/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08b3e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8b3e6(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  candidate_4256();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8848;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8848;
  candidate_1ad6e();
  candidate_1aca2();
  *unaff_A6 = 0x8b43e;
  bVar3 = false;
  candidate_409a();
  if (bVar3) {
    if (((*(byte *)((int)unaff_A6 + 0x7f) & 1) == 0) && ((*(byte *)(unaff_A6[3] + 0x7f) & 1) != 0))
    {
      *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) | 1;
      candidate_4256();
    }
    if ((*(byte *)(unaff_A6[3] + 0x7f) & 2) == 0) {
      candidate_1abc8();
      candidate_8b52a();
      candidate_1ad8c();
      candidate_427a();
      return;
    }
    candidate_4256();
    candidate_1ad52();
    candidate_1b1dc();
    *unaff_A6 = 0x8b4d6;
    uVar4 = false;
    candidate_409a();
    if ((bool)uVar4) {
      candidate_1abc8();
      candidate_8b52a();
      candidate_1ad8c();
      candidate_427a();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_8b502;
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
