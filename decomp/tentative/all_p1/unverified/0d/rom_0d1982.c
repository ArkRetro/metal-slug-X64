/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1982.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d1982(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0xffff;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x862;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x862;
  candidate_9ea8();
  candidate_1ad6e();
  *(undefined2 *)(unaff_A6 + 0xb) = 8;
  candidate_4256();
  *unaff_A6 = 0xd19d0;
  candidate_d1b6c();
  candidate_d1b28();
  cVar1 = *(char *)(unaff_A6[3] + 0x60);
  if (cVar1 == '\x01') {
    *unaff_A6 = candidate_d19f0;
  }
  bVar2 = cVar1 == '\0';
  candidate_19424();
  if (bVar2) {
    *unaff_A6 = candidate_d1ac6;
  }
  return;
}
