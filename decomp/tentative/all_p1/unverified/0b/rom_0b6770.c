/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b6770.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b6770(void)

{
  char cVar1;
  undefined4 *unaff_A6;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x87e8;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x87e8;
  candidate_1ad8c();
  candidate_4256();
  *unaff_A6 = 0xb67a6;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  cVar1 = *(char *)(unaff_A6[3] + 0x79);
  if (cVar1 != *(char *)((int)unaff_A6 + 0x79)) {
    *(char *)((int)unaff_A6 + 0x79) = cVar1;
    if (cVar1 == '\x01') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_b6202;
  }
  return;
}
