/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bc42c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bc42c(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(char *)((int)unaff_A6 + 0x7a) = *(char *)((int)unaff_A6 + 0x7a) + '\x01';
    if (4 < *(byte *)((int)unaff_A6 + 0x7a)) {
      candidate_bc38e();
      candidate_1da40();
      candidate_120f8();
      candidate_19dfc();
      candidate_bc550();
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    candidate_120f8();
  }
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_bc542();
  return;
}
